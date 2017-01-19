import os

__author__ = 'Guilherme'

PATH_DATA = "C:/Users/Gui/Desktop/MTSP/Instancias/Resultados/"

def analyze_txt(name, file_to_result):
    f = open(PATH_DATA + name, "r")
    switches = 0
    times = 0
    counter = 0
    for line in f:
        line = line.replace("\n", "")
        data = line.split(" * ")
        numbers = data[-1].split("/")
        switches += float(numbers[0])
        times += float(numbers[1])
        counter += 1

    f.close()

    if counter > 0:
        average_switches = switches/counter
        average_times = times/counter
        
        line_to_write = name.replace(".txt", "") + " * Trocas: " + str("%0.2f" % average_switches) + " - Tempo Medio: " + str("%0.2f" %average_times) + " milliseconds \n"
        file_to_result.write(line_to_write)


      


def analyze_all_txts():
    files = os.listdir(PATH_DATA)
    f = open("results.txt", "w")
    for name in files:
        analyze_txt(name, f)

    f.close()

analyze_all_txts()