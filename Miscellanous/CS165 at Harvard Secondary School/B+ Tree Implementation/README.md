This B+ Tree is implemented by Keyang Lu, using the starter code from CS165 A1 (Assignment 1).
This assignment is for the course CS165 Data Systems and Machine Learning of the Harvard Secondary School Program.
Writeup.pdf is a brief documentation and analysis of my particular B+ Tree data structure implemetation.
To compile: make clean; make;
To run test.txt: ./main -f tests/test.txt

test.txt is a general test of 5M puts and 500K gets
test1.txt is a test to test the correctness of replacing pre-existing keys
test2.txt is a test to test cases where 0 is involved
test3.txt is a test to test boundary cases
