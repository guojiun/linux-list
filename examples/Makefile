CC = gcc
NUMBER = 50000

all: insert-sort.c quick-sort.c merge-sort.c
	$(CC) -o insert-sort insert-sort.c -DNAME=\"insert.log\" -DN=$(NUMBER)
	$(CC) -o quick-sort quick-sort.c   -DNAME=\"quick.log\"	 -DN=$(NUMBER)
	$(CC) -o merge-sort merge-sort.c   -DNAME=\"merge.log\"  -DN=$(NUMBER)

test: insert-sort quick-sort merge-sort
	@./insert-sort
	@./quick-sort  
	@./merge-sort 

PHONY: clean
clean:
	rm -rf insert-sort quick-sort merge-sort *.log