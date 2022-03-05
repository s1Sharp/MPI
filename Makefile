of = output\output
exe =  executable\Release\task
n=4
numberofprocess=$(n)
run = mpiexec -n $(numberofprocess)
INT_MAX = 2147483647

task3_options_number_iter=1000000
task5_options_mas_size=1000

all: mkdir task1 task2 task3 task4 task5 task6 task7 task8 task9 task10 task11 task12 task13

mkdir:
	IF exist output ( echo output exists ) ELSE ( mkdir output && echo output created)
	
task1: mkdir
	$(run) $(exe)1.exe > $(of)1.txt

task2: mkdir
	$(run) $(exe)2.exe  > $(of)2.txt

task3: mkdir
	$(run) $(exe)3.exe $(task3_options_number_iter) > $(of)3.txt

task4: mkdir
	$(run) $(exe)4.exe  > $(of)4.txt

task5: mkdir
	$(run) $(exe)5.exe $(task5_options_mas_size) > $(of)5.txt

task6: mkdir
	$(run) $(exe)6.exe  > $(of)6.txt

task7: mkdir
	$(run) $(exe)7.exe  > $(of)7.txt

task8: mkdir
	$(run) $(exe)8.exe  > $(of)8.txt

task9: mkdir
	$(run) $(exe)9.exe  > $(of)9.txt

task10: mkdir
	mpiexec -n 2 $(exe)10.exe  > $(of)10.txt

task11: mkdir
	$(run) $(exe)11.exe  > $(of)11.txt

task12: mkdir
	$(run) $(exe)12.exe  > $(of)12.txt

task13: mkdir
	$(run) $(exe)13.exe  > $(of)13.txt