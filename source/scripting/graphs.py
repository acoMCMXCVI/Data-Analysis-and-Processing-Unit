import csv
import numpy as np
from scipy.fftpack import fft
import matplotlib.pyplot as plt


acc = []
pos = []

with open('../face_landmark_remote_controller/Data/out.tsv') as tsvfile:
  reader = csv.reader(tsvfile, delimiter='\t')
  for row in reader:
    acc.append(float(row[0]))
    pos.append(float(row[1]))

t = np.arange(len(acc))

fig, axs = plt.subplots()
fig.suptitle('Position')

axs.plot( t, pos, label="pos" )

axs.set_xlabel('time')
axs.set_ylabel('pos')
axs.grid(True)

axs.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc='lower left', borderaxespad=0.)

#TODO axs[1].legend(bbox_to_anchor=(0., 1.02, 1., .102), loc='lower left', borderaxespad=0.)

N = 512



def fft_plot_range(start, data, name):

    accx = np.array(data[start:start+N]) #TODO acc->pos
    t2 = np.arange(len(accx))

    accX = fft(accx)
    F = np.linspace(0.0, 0.5, N//2+1)

    accA = 2.0/N * np.abs(accX[0:N//2+1])


    axs[0].plot(t2, accx, label="x for {} for start = {}".format(name,start))
    axs[0].set_xlim([0, N-1])
    axs[0].set_ylim([-30, 30])
    axs[0].legend(bbox_to_anchor=(0., 1.02, 1., .102), loc='lower left', borderaxespad=0.)

    axs[1].plot(F, accA, label="A for {} ".format(name))
    axs[1].set_xlim([0, 0.5])
    axs[1].set_ylim([0, 10])
    axs[1].legend(bbox_to_anchor=(0., 1.02, 1., .102), loc='lower left', borderaxespad=0.)


def average (arr,num, start):

    arrAvg = np.array([])

    arr_range = np.array(arr[start:start+N])
    arr_rangeZeros = np.zeros(num-1)
    arr_rangeZeros = np.append (arr_rangeZeros, arr_range )

    for a in range(num,arr_rangeZeros.size+1):
        arrAvg = np.append(arrAvg, np.sum(arr_rangeZeros[a-num:a])/num)


    t = np.arange(arrAvg.size)

    fig, axs = plt.subplots()
    fig.suptitle("Average with lenght = {}".format(num))

    axs.plot( t, arrAvg, label="average pos" )
    axs.plot( t, arr_range, label="pos" )

    axs.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc='lower left', borderaxespad=0.)

    return arrAvg


def velocity (position,start,divide,show):

    time = 1./30

    velCalc = np.array([])

    posArrayBase = np.array(position[start:start+N]);
    posArray = np.array(posArrayBase)
    posArray = np.insert(posArray,0,posArray[0])

    for a in range(1,posArray.size):
        velCalc = np.append(velCalc, ((posArray[a]-posArray[a-1])/time)/divide )


    if show == True:
        t = np.arange(velCalc.size)

        fig.suptitle("Position, velocity and acceleration with start = {}".format(start))

        axs.plot( t, velCalc, label="vel" )
        axs.plot( t, posArrayBase, label="pos" )

        axs.grid(True)

        axs.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc='lower left', borderaxespad=0.)

    return velCalc



def acceleration (velocity,divide,show):

    time = 1./30

    accCalc = np.array([])

    velArrayBase = np.array(velocity);
    velArray = np.array(velArrayBase)
    velArray = np.insert(velArray,0,velArray[0])

    for a in range(1,velArray.size):
        accCalc = np.append(accCalc, ((velArray[a]-velArray[a-1])/time)/divide )

    if show == True:
        t = np.arange(accCalc.size)

        axs.plot( t, accCalc, label="acc" )

        axs.grid(True)

        axs.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc='lower left', borderaxespad=0.)

    return accCalc



fig, axs = plt.subplots()
acceleration(velocity(pos,500,5,1),100,1)



start = 500

arr_average7 = average (pos,7,start)
arr_average3 = average (pos,3,start)

fig, axs = plt.subplots()
acceleration(velocity(arr_average7,0,5,1),100,1)

fig, axs = plt.subplots(2, 1)
fig.suptitle("FFT")

fft_plot_range(0, arr_average3, "average 3")
fft_plot_range(0, arr_average7, "average 7")
fft_plot_range(start, pos, "position")


plt.show()
