# -*- coding: utf-8 -*-
#Criado por Mislene Nunes
#Modificado por Igor Nascimento

import sys
import matplotlib.pyplot as plt
import matplotlib.tri as tri
from mpl_toolkits.mplot3d import Axes3D
import mpl_toolkits.mplot3d as a3
from matplotlib import cm

from numpy import linalg as LA
import numpy as np

from sklearn.decomposition import PCA
from sklearn import manifold

from scipy.spatial import Delaunay
from scipy.spatial import ConvexHull

from ctypes import cdll
from ctypes import *

#Leitura do arquivo e armazenamento em uma matriz
def readFile(nameFile,row,column):
    File = open(nameFile, "r")
    matrixA = np.zeros((row,column))
    i=0
    for line in File:
        values = line.split()
        matrixA[i]=values
        i=i+1        
    File.close()
    return matrixA

#Determina pontos intermediarios de acordo com os dois materiais escolhidos
#e um parametro t que deve ter seu valor entre 0 e 1
def SubRetas(p,q,t):
    lin = len(t)
    col = len(p)
    f = np.zeros((lin,col))
    for i in range(lin):
        fp = (1 - t[i])*p + t[i]*q
        f[i,:] = fp
    return f

#Validacao do ponto (material) em um simplexo e determinacao
#das coordenadas baricentricas de acordo com o simplexo e o ponto (material) dado
def CoordenadasBa(p,simplexo,Y):
	dim = len(p) + 1
	ret=0
	M  = np.ones((dim,dim))	
	vp = np.ones((dim,1))
	vp[:dim-1,0] = p.T
	
	for i in range(dim):
		indice = simplexo[i]
		pIndice = Y[indice,:]
		M[i,:dim-1] = pIndice
	
	M = M.T
	X = LA.solve(M,vp)
	#print "coordenadas Baricentricas",X
	#print np.sum(X)

	xmin = np.min(X)
	xmax = np.max(X)
	inCoor = 0        
	epsilon = 1.0e-10
	if(xmin<-epsilon or xmax>(1.0+epsilon)):
	 #print X
	 inCoor = 1
        
	return X,inCoor

#responsavel pela reconstrucao do ponto	(material)
def Ponto(simplexo,coordenadas,Y):
    lin = len(coordenadas)
    m,n = Y.shape
    p = np.zeros((n))
    ys = Y[simplexo]
    aux = 0
    for i in range(n):
        for j in range(lin):
            aux+= coordenadas[j]*ys[j][i]
        p[i] = aux
        aux = 0
    return p

#responsavel pela reconstrucao do ponto (material)
def Reconstrucao(coordenada_ponto,simplexo,iRecon):
    reconstrucao_ponto = Ponto(simplexo,coordenada_ponto,iRecon)
    return reconstrucao_ponto

#Transformacao do material que esta em uma linha da matriz para um vetor
#Com o objetivo de facilitar na escrita em binario
def Passagem(vecc, Material, column):
    for i in range(column):
        vecc[i] = Material[i]

    return vecc

#lendo o indice dos materiais
m1 = int(sys.argv[1])
m2 = int(sys.argv[2])

#para as matrizes apos a reducao de dimensionalidade
row = 100 #numero de materiais
column = 3 #numero de amostras 

#para as matrizes em sua dimensao original
row2 = 100 #numero de materiais
column2 = 1458000 #numeros de amostras

#Leitura das matrizes em dimensao reduzida
YpcaR= readFile("Matrix_A_PCA_Red.txt",row,column)
YpcaG = readFile("Matrix_A_PCA_Green.txt",row,column)
YpcaB = readFile("Matrix_A_PCA_Blue.txt",row,column)

#Triangulacao e feita para cada canal
#p representa um material
#q representa outro material da base
#os indices representa uma linha da matriz (0-99) e um material qualquer escolhido
triR = Delaunay(YpcaR)
pR = triR.points[m1] 
qR = triR.points[m2] 

triG = Delaunay(YpcaG)
pG = triG.points[m1] 
qG = triG.points[m2] 

triB = Delaunay(YpcaB)
pB = triB.points[m1] 
qB = triB.points[m2] 

#https://docs.scipy.org/doc/scipy-0.14.0/reference/generated/scipy.spatial.Delaunay.html

#t = np.array([0.0, 0.2, 0.4, 0.6, 0.8, 1.0])
h = 1.0/50.0 #determinacao da distancia entre um ponto e outro
t = np.arange(0,h+1,h) #determinacao do array que conterar todos os parametros t determinados utilizando a distancia h
#t = np.array([0.00005,0.0001,0.0005,0.0008,0.001,0.002])
#t = np.array([0.0, 0.02, 0.04, 0.1, 0.2, 0.3])

#Cria uma reta para cada canal (responsavel pela navegacao)
retaR = SubRetas(pR,qR,t)
retaG = SubRetas(pG,qG,t)
retaB = SubRetas(pB,qB,t)  

#Tamanho da reta para saber quantos valores de t foram definidos 
tam = len(retaR)
simR = []
simG = []
simB = []
#encontra o simplexo para cada ponto (material) da reta de cada canal
for i in range(tam):
    sR = triR.find_simplex(retaR[i])
    sG = triG.find_simplex(retaG[i])
    sB = triB.find_simplex(retaB[i])
    simR.append(sR)
    simG.append(sG)
    simB.append(sB)
    #print "indice e simplexo", i, s
    #XR = CoordenadasBa(retaR[i],tri.simplices[sR],YpcaR)
    #XG = CoordenadasBa(retaG[i],tri.simplices[sG],YpcaG)
    #XB = CoordenadasBa(retaB[i],tri.simplices[sB],YpcaB)
    
#j = 5

#M1R = readFile("Matrix_A_Red.txt",row2,column2)
#M1G = readFile("Matrix_A_Green.txt",row2,column2)
#M1B = readFile("Matrix_A_Blue.txt",row2,column2)
    
#Biblioteca responsavel por escrever ja o arquivo em binario
libbin = cdll.LoadLibrary("./libbin.so")

for j in range(25,26):
    #Validacao do simplexo e encontra coordenadas baricentricas do ponto (material)
    #de acordo com o parametro t
    XR,inCoorR = CoordenadasBa(retaR[j],triR.simplices[simR[j]],YpcaR)
    
    XG,inCoorG = CoordenadasBa(retaG[j],triG.simplices[simG[j]],YpcaG)
    
    XB,inCoorB = CoordenadasBa(retaB[j],triB.simplices[simB[j]],YpcaB)

    #Validacao se o ponto (material) esta dentro do simplexo
    #So cria material se estiver dentro do simplexo
    if(inCoorR == 1 or inCoorG == 1 or inCoorB == 1):
        print "fora das coordenadas"
    else:
        #Leitura das matrizes original para cada canal, para fazer
        #a interpolacao e criar o novo material
        #apos leitura e reconstrucao do material para o canal
        #libera memoria, pois consome muita memoria
        
        M1R = readFile("Matrix_A_Red.txt",row2,column2)
        MaterialR = Reconstrucao(XR,triR.simplices[simR[j]],M1R)
        del M1R

        M1G = readFile("Matrix_A_Green.txt",row2,column2)
        MaterialG = Reconstrucao(XG,triG.simplices[simG[j]],M1G)
        del M1G

        M1B = readFile("Matrix_A_Blue.txt",row2,column2)
        MaterialB = Reconstrucao(XB,triB.simplices[simB[j]],M1B)
        del M1B

        #responsavel por gerar os materiais ja em binario
        #libbin = cdll.LoadLibrary("./libbin.so")
        sample = libbin.sample_bin

        vector = c_double*column2

        sample.argtypes = [c_char_p,POINTER(vector),POINTER(vector),POINTER(vector)]
        
        auxR = vector()
        auxG = vector()
        auxB = vector()

        veccR = Passagem(auxR,MaterialR,column2)
        veccG = Passagem(auxG,MaterialG,column2)
        veccB = Passagem(auxB,MaterialB,column2)
        #Salva os materiais em binarios nas pasta criados
        sample("New/material_"+str(j)+".binary",pointer(veccR),pointer(veccG),pointer(veccB))









