PT_SEM_WAIT(pt, &mutex);    \ 
estado[i] = FAMINTO;    \
INTENCAO(pt, i);    \
PT_SEM_SIGNAL(pt, &mutex);  \
PT_SEM_WAIT(pt, &(mux_f[i]));   \