import multiprocessing, time, random
import sys
import os

def job():
  #print('work')
  os.system('./test -h 10.7.18.26 -t 4 -p 12345 -f ./jpgfiles')
  #os.system('./test -t 1 -p 11233 -f ./jpgfiles')

if __name__== '__main__':
  num_processes = int(sys.argv[1])
  processes = []
  for i in range(num_processes):
    p = multiprocessing.Process(target=job, args = ())
    p.start()
    time.sleep(random.randint(0, 5))
    processes.append(p)
  for i in range(num_processes):
    processes[i].join()
