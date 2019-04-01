# Train a BDT  to classify signal and background events 
# Data stored in Root ntuple (HEP specific data format)
# kehinde.tomiwa4@gmail.com

import pandas as pd
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
from root_numpy import root2array, rec2array, array2root
from sklearn.utils import shuffle
from sklearn.ensemble import AdaBoostClassifier,GradientBoostingClassifier
from sklearn.metrics import classification_report, confusion_matrix
#from sklearn.model_selection import train_test_split
from sklearn.cross_validation import train_test_split

# Variable names 
branch_names = """diff_sumpt2, m_yy, Vertex_sumpt2_php, Vertex_sumpt2_Pileup, sumet, dz, dphi_php_met, dphi_pu_met, metsig,met,dphiyymet,weight""".split(",")
branch_names = [c.strip() for c in branch_names] 
branch_names = list(branch_names)

signal = root2array("Signal.root","tree",branch_names)
signal = rec2array(signal)
backgr = root2array("Background.root","tree",branch_names)
backgr = rec2array(backgr)


def makecompare_plots(Svar,Bvar,Sw,Bw,xlab,binsrang,nbin,norm=False):
    plt.figure(figsize=(10,6))
    hist1 = np.histogram(Svar,bins=nbin,range=binsrang,weights=Sw,normed=norm)
    hist2 = np.histogram(Bvar,bins=nbin,range=binsrang,weights=Bw,normed=norm)
    plt.step(hist1[1][:-1], hist1[0],color="b", label="signal")
    plt.step(hist2[1][:-1], hist2[0],color="r", label="background")
    plt.xlabel(xlab)
    plt.ylabel('Event')
    plt.legend(loc="best")
    


X_weight = np.concatenate((signal,backgr))
X_weight.shape
signal.shape[0]
backgr.shape[0]
y_tem = np.concatenate((np.ones(signal.shape[0]),np.zeros(backgr.shape[0])))
#y_tem
data = np.hstack((X_weight,y_tem.reshape(y_tem.shape[0],-1)))
#data
feat_name = branch_names+['target']
#feat_name
df = pd.DataFrame(data,columns=feat_name)
#df.head()
#df.tail()
df = shuffle(df)
X = df.drop(['weight','target'],axis=1)
#X.head()

y=df['target']
y.count()
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.50, random_state=101)

#y_test.describe()

shrinkage = [] 
test_error_rate = []
train_error_rate = []
LR = np.arange(0.01, 1.0, 0.01)
for i in range(len(LR)):
  print(i,LR[i])
  gbc = GradientBoostingClassifier(loss='deviance',learning_rate=LR[i],n_estimators=400,max_depth=2)
  gbc.fit(X_train,y_train)
  y_pred = gbc.predict(X_test)
  ytrain_pred = gbc.predict(X_train)
  shrinkage.append(LR[i])
  test_error_rate.append(np.mean(y_pred != y_test))
  train_error_rate.append(np.mean(ytrain_pred != y_train))

  
#print('test',test_error_rate)
#print('train',train_error_rate)

# write output to a csv file
df3 = pd.DataFrame(data=dict(shrinkage=shrinkage,test_error_rate=test_error_rate,train_error_rate=train_error_rate))
df3.to_csv('/afs/cern.ch/work/k/ktomiwa/public/Python/rateout',index=False)



