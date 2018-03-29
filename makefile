all:
	$(MAKE) grammar
	$(MAKE) lex
	g++ -std=c++11 -Wall -Wextra grammar.tab.c lex.yy.c main.c

grammar:
	bison -d grammar.y

lex:
	flex lex.l

teszt:
	$(MAKE) all
	 ./a.out < ./test/enter.code 
