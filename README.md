session 1 (12.10) vystup:

IFJ projekt TODO:
1.) Lexicalna analzya 

enum TOK = {TOK_KEYWORD, TOK_ID, TOK_LIT...}

struct Token {
  enum tok;
  char *value;
};

Zakladna kostra programu:
  prechadzam kod char po chare a tokenizujem 

implementacia DKA na tokenizaciu lexemov:
- graficky znazornit DKA(together) 
- DKA automat implementovat pomocou struktury + case switch
- tokenizacia zdrojoveho kodu - kazdy sekvenciu charakterov nahradime tokenom (keyword, identifier, literal...)
- tokeny ulozime do pola za sebou


TODO until next session:
-nakreslit DKA (podla zadania) 

TODO on next session: 
