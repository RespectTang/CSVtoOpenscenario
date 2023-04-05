import csv

v1 = []
v2 = []
dis = []
with open('C:/Users/RTT/Desktop/ICTIS2023/cutData/testvvd.csv', 'r') as f:
    reader = csv.reader(f)
    #print(type(reader))
    i = 0
    for row in reader:
        
        print(row)
