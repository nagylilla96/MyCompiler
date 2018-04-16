all:
	$(MAKE) grammar
	$(MAKE) lex
	g++ -std=c++11 -Wall -Wextra calc.tab.c lex.yy.c

grammar:
	bison -d calc.y

lex:
	flex calc.l

teszt:
	$(MAKE) all
	./a.out < ./test/enter.code
