#include <stdio.h>

int comentario_longo(char *c, int *pos);

int main()
{   int resultado;
    int pos;

    char c[] = "--[[ Testando 1 2 3 \n oláaaa]]";
    pos = 0;
    
    resultado = comentario_longo(c, &pos);
    printf("Resultado: %d\n", resultado);
    printf("Posição final: %d\n", pos);
    
    char d[] = "--[[ Testando 1 2 3 \n oláaaa";
    pos = 0;
    
    resultado = comentario_longo(d, &pos);
    printf("Resultado: %d\n", resultado);
    printf("Posição final: %d\n", pos);
    
    return 0;
}

int comentario_longo(char *c, int *pos) {
    int inicio = *pos;
    
    if (c[*pos] == '-' && c[*pos + 1] == '-' && c[*pos + 2] == '[' && c[*pos + 3] == '[') {
        *pos += 4; 
        
        while (c[*pos] != '\0') {
            if (c[*pos] == ']' && c[*pos + 1] == ']') {
                *pos += 2;
                return 1;
            } 
            else {
                (*pos)++;
            }
        }
    }
    *pos = inicio; 
    return 0; 
}
