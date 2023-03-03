import os
import numpy as np
from matplotlib import pyplot as plt
from scipy import stats
import sys
from itertools import combinations
import re
from datetime import datetime, timedelta
import netCDF4

def read_cbr_file(filename,pars):
    with open(filename, 'rb') as fid:
        BD = np.fromfile(fid, np.float64)

    N = int(BD.shape[0]/pars)
    PARS = BD.reshape((N,pars))
    burn = int(PARS.shape[0]/2)
    return PARS[burn:,:]

def get_dates(C_dates):
    dates = []
    stardate = datetime(year = 2001,month = 1, day = 1)
    for dat in C_dates:
        dates+=[stardate+timedelta(days = int(dat))]
    return np.array(dates)



def gelman_rubin(x, return_var=False):

    if np.shape(x) < (2,):
        raise ValueError(
            'Gelman-Rubin diagnostic requires multiple chains of the same length.')

    try:
        m, n = np.shape(x)
    except ValueError:
        return [gelman_rubin(np.transpose(y)) for y in np.transpose(x)]
    # Calculate between-chain variance
    B_over_n = np.sum((np.mean(x, 1) - np.mean(x)) ** 2) / (m - 1)
    # Calculate within-chain variances
    W = np.sum(
        [(x[i] - xbar) ** 2 for i,
         xbar in enumerate(np.mean(x,
                                   1))]) / (m * (n - 1))
    # (over) estimate of variance
    s2 = W * (n - 1) / n + B_over_n
    
    if return_var:
        return s2
    # Pooled posterior variance estimate
    V = s2 + B_over_n / m
    # Calculate PSRF
    R = V / W

    return np.sqrt(R)


def DEMCMC_conv(f,dim):
    meangrs = []
    fracs = []
    masswalker = []
    cbr = read_cbr_file(f[0],dim)
    for walks in range(200):
        walker = []
        walker = cbr[np.arange(walks,cbr.shape[0],100),:]
        walker = np.array(walker)

        masswalker+= [walker]

        cbrl_1 = walker[0:int(walker.shape[0]/2),:]
        cbrl_2 = walker[int(walker.shape[0]/2):int(walker.shape[0]/2)*2,:]
        gr = []
        for i in range(cbrl_1.shape[1]):
                tgr = gelman_rubin([cbrl_1[:,i],cbrl_2[:,i]])
                gr+=[tgr]

        gr = np.array(gr)
        fracs+=[ len(gr[gr<1.2])/len(gr)]
        meangrs+= [np.mean(gr)]

        


    gr = []
    shapes = [mw[:,i].shape[0] for mw in masswalker]
    for i in range(masswalker[0].shape[1]):
            forthegr = [mw[0:min(shapes),i] for mw in masswalker]
            tgr = gelman_rubin(forthegr)
            gr+=[tgr]
    gr = np.array(gr)

    return(np.array(fracs),np.array(meangrs),np.array(gr))

def MHMCMC_conv(f,dim):
    maxx = 0;
    mincomb = None
    frac = None
    meangr = None
    for comb in f:
        cbr_load_1 = read_cbr_file(comb[0],dim)
        cbr_load_2 = read_cbr_file(comb[1],dim)
        cbr_load_3 = read_cbr_file(comb[2],dim)
        cbr_load_4 = read_cbr_file(comb[3],dim)
        gr = []
        for i in range(cbr_load_1.shape[1]):
                tgr = gelman_rubin([cbr_load_1[:,i],cbr_load_2[:,i],cbr_load_3[:,i],cbr_load_4[:,i]])
                # tgr = gelman_rubin([cbr_load_1[:,i],cbr_load_3[:,i],cbr_load_2[:,i]])

                gr+=[tgr]
        gr = np.array(gr)
        if len(gr[gr<1.2])/len(gr)>maxx:
            maxx = len(gr[gr<1.2])/len(gr)
            mincomb = comb
            frac = len(gr[gr<1.2])/len(gr)
            meangr = np.mean(gr)
    return (mincomb , meangr,maxx)


def numpars(c_loc,modelid):
    tf = open(c_loc+"C/projects/CARDAMOM_MODELS/DALEC/DALEC_"+str(modelid)+"/DALEC_"+str(modelid)+".c", "r")
    d = tf.read()
    tf.close()

    noparss = re.findall('nopars(.*);',d)[0]
    return int(re.findall('\d+', noparss )[0])


def cbr_info(c_loc,modelid):
    if os.path.exists(c_loc+"C/projects/CARDAMOM_MODELS/DALEC/DALEC_"+str(modelid)+"/DALEC_"+str(modelid)+"_INDICES.c"):
        tf = open(c_loc+"C/projects/CARDAMOM_MODELS/DALEC/DALEC_"+str(modelid)+"/DALEC_"+str(modelid)+"_INDICES.c", "r")
        d = tf.read()
        tf.close()

    else:
        tf = open(c_loc+"C/projects/CARDAMOM_MODELS/DALEC/DALEC_"+str(modelid)+"/DALEC_"+str(modelid)+".c", "r")
        d = tf.read()
        tf.close()

    tf2 = open(c_loc+"C/projects/CARDAMOM_MODELS/DALEC/DALEC_"+str(modelid)+"/PARS_INFO_"+str(modelid)+".c", "r")
    parinfo = {}
    for l in tf2.readlines():
        if "P." in l and "//" not in l:
            parname = re.findall('P.(.*)]',l)[0]
            bound = re.findall('=(.*);',l)[0]
            if(parname not in parinfo.keys()):
                parinfo[parname] = [eval(bound),-9999]
            else:
                parinfo[parname][1] = eval(bound)
    tf2.close()

    s1 = d.find("DALEC_"+str(modelid)+"_PARAMETERS")
    e1 = d.find('}',s1)
    pname = re.findall('int (.*);',d[s1:e1])
    s2 = d.find('{',e1)
    e2 = d.find('}', s2)
    pind = [int(a) for a in re.findall('[0-9]+', d[s2+1:e2])] 
    for a,b in list(zip(pname,pind)):
        parinfo[a] = [b]+parinfo[a]


    return parinfo


def flux_info(c_loc,modelid):
    if os.path.exists(c_loc+"C/projects/CARDAMOM_MODELS/DALEC/DALEC_"+str(modelid)+"/DALEC_"+str(modelid)+"_INDICES.c"):
        tf = open(c_loc+"C/projects/CARDAMOM_MODELS/DALEC/DALEC_"+str(modelid)+"/DALEC_"+str(modelid)+"_INDICES.c", "r")
        d = tf.read()
        tf.close()

    else:
        tf = open(c_loc+"C/projects/CARDAMOM_MODELS/DALEC/DALEC_"+str(modelid)+"/DALEC_"+str(modelid)+".c", "r")
        d = tf.read()
        tf.close()

    s1 = d.find("DALEC_"+str(modelid)+"_FLUXES")
    e1 = d.find('}',s1)
    fname = re.findall('int (.*);',d[s1:e1])
    s2 = d.find('{',e1)
    e2 = d.find('}', s2)
    find = [int(a) for a in re.findall('[0-9]+', d[s2+1:e2])] 
    return dict(zip(find,fname))


def pool_info(c_loc,modelid):
    if os.path.exists(c_loc+"C/projects/CARDAMOM_MODELS/DALEC/DALEC_"+str(modelid)+"/DALEC_"+str(modelid)+"_INDICES.c"):
        tf = open(c_loc+"C/projects/CARDAMOM_MODELS/DALEC/DALEC_"+str(modelid)+"/DALEC_"+str(modelid)+"_INDICES.c", "r")
        d = tf.read()
        tf.close()

    else:
        tf = open(c_loc+"C/projects/CARDAMOM_MODELS/DALEC/DALEC_"+str(modelid)+"/DALEC_"+str(modelid)+".c", "r")
        d = tf.read()
        tf.close()

    s1 = d.find("DALEC_"+str(modelid)+"_POOLS")
    e1 = d.find('}',s1)
    pname = re.findall('int (.*);',d[s1:e1])
    s2 = d.find('{',e1)
    e2 = d.find('}', s2)
    pind = [int(a) for a in re.findall('[0-9]+', d[s2+1:e2])] 
    return dict(zip(pind,pname))

def get_flux(file):
    ffs = np.array(netCDF4.Dataset(file)['FLUXES'])
    return ffs[int(ffs.shape[0]/2):,:,:]

def get_pool(file):
    pools = np.array(netCDF4.Dataset(file)['POOLS'])
    return pools[int(pools.shape[0]/2):,:,:]



def assim_data_info(c_loc,modelid):
    tf = open(c_loc+"C/projects/CARDAMOM_MODELS/DALEC/DALEC_"+str(modelid)+"/DALEC_"+str(modelid)+".c", "r")
    d = tf.read()
    tf.close()

    pname = re.findall('OBSOPE.SUPPORT_(.*)_',d)

    return pname

def rm99(xx):
    x =xx.copy()
    x[x==-9999.0] = np.nan
    return x


def assim_data_fluxes(c_loc,modelid):
    tf = open(c_loc+"C/projects/CARDAMOM_MODELS/DALEC/DALEC_"+str(modelid)+"/DALEC_"+str(modelid)+".c", "r")
    d = tf.read()
    tf.close()

    s1 = d.find("INITIALIZE_OBSOPE_SUPPORT(&OBSOPE);")
    e1 = d.find('return 0;}',s1)
    info = d[s1:e1]
    info = info[info.rfind('OBSOPE.SUPPORT')+14:]
    
    f_info = flux_info(c_loc,modelid)
    invfluxinfo = {v: k for k, v in f_info.items()} 
    

    ffs = {}
    for rr in re.findall('OBSOPE.(.*);',info):
        if('fluxes' in rr and '_n_' not in rr):
            fluxn = rr[:rr.index('_')]
            mf = re.findall(fluxn+'_flux_signs\[\](.*);',info)[0]
            
            fluxlocs = []
            for aa in re.findall(fluxn+'_fluxes\[(.*);',info) :
                if('=' in aa):
                    fluxlocs+=[int(invfluxinfo[aa[aa.index('.')+1:]])]
            
            
            multsigns = re.findall('-?\d+\.?\d*',mf)
            multsigns = [eval(m) for m in multsigns]
            ffs[fluxn] = [fluxlocs,multsigns]
        elif('flux' in rr and 'signs' not in rr and 'fluxes' not in rr):
            ffs[rr[:rr.index('_')]] =  invfluxinfo[rr[rr.index('.')+1:]]
        
    return ffs


def assim_data_pools(c_loc,modelid):
    tf = open(c_loc+"C/projects/CARDAMOM_MODELS/DALEC/DALEC_"+str(modelid)+"/DALEC_"+str(modelid)+".c", "r")
    d = tf.read()
    tf.close()

    s1 = d.find("INITIALIZE_OBSOPE_SUPPORT(&OBSOPE);")
    e1 = d.find('return 0;}',s1)
    info = d[s1:e1]
    info = info[info.rfind('OBSOPE.SUPPORT')+14:]

    pinfo = pool_info(c_loc,modelid)
    invpoolinfo = {v: k for k, v in pinfo.items()} 

    pps = {}
    for rr in re.findall('OBSOPE.(.*);',info):
        if('pools' in rr and '_n_' not in rr):
            pooln = rr[:rr.index('_')]
           

            poollocs = []
            for aa in re.findall(pooln+'_pools\[(.*);',info) :
                if('=' in aa):
                    poollocs+=[int(invpoolinfo[aa[aa.index('.')+1:]])]


    
            pps[pooln] = poollocs
        elif('pool' in rr and 'signs' not in rr and 'pools' not in rr):
            pps[rr[:rr.index('_')]] =  invpoolinfo[rr[rr.index('.')+1:]]

    return pps

def pltoutput(x,y,color,label=""):
    sd = np.percentile(y,[25,75],axis = 0)
    plt.plot(x,np.median(y,axis = 0),color = color, label = label)
    plt.fill_between(x, sd[0,:], sd[-1,:], color=color, alpha=.3)
    plt.xticks(rotation=45)
    return np.median(y,axis = 0)

def pltassimdata(x,y):
    if(np.sum(~np.isnan(y)) < len(y)/10):
        plt.scatter(x,y,color = 'black')
    elif(np.sum(~np.isnan(y)) ):
        plt.plot(x,y,color = 'black')
