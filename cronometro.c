int contador;
const int display[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

void main(){
    contador=0;

    TRISB.RB0 = 1; // Configura RB0 como um bot�o de entrada
    TRISB.RB1 = 1; // Configura RB1 como um bot�o de entrada
    TRISD = 0x00; // Configura RD como sa�da (display de 7 segmentos)
    PORTD = 0x00; // Configura a porta D como sa�da

    INTCON2.RBPU=0;

    RCON.IPEN = 1; // Habilida n�veis de interrup��o
    INTCON.GIEH = 1; // Chave geral de alta prioridade se IPEN = 1

    // Configura��o da interrup��o externa INTO
    INTCON.INT0IE = 1; // Habilita interrup��o externa INT0
    INTCON.INT0IF = 0; // Flag para adicionar interrup��o INT0
    INTCON2.INTEDG0 = 1; // Borda, 1 � a borda de subida ao soltar a tecla
                         // 0 � a borda de descida ao pressionar a tecla

    // Configura��o do temporizador TWR0
    INTCON.TMR0IE = 1; // Habilita a interrup��o associada ao temporizador TMR0
    INTCON.TMR0IF = 0; // Zera a flag.
    T0CON = 0b00000110; // Registro de controle do temporizador

    // Configura��o do temporizador TMR1
    PIE1.TMR1IE = 1; // Habilita interrup��o do temporizador TMR1
    PIR1.TMR1IF = 0; // Zera a flag de interrup��o do temporizador TMR1
    IPR1.TMR1IP = 1; // Prioridade alta para TMR1
    T1CON = 0b00110000; // Configura o registrador T1CON, referente ao temporizador TMR1
                        // Prescaler 1:16, modo 16 bits

    // Overflow
    TMR1H = 0x0B; // Primeiro byte do TMR1
    TMR1L = 0xDC; // �ltimo byte do TMR1

    TMR0H=0xC2;
    TMR0L=0xF7;


    while(1);
}

void INTERRUPTION_HIGH() iv 0x0008 ics ICS_AUTO {
    if (INTCON.INT0IF) { // se a interrup��o amostrada foi a INT0
        // l�gica para o primeiro bot�o (RB0)
        T0CON.TMR0ON = 1; // Inicia o timer TMR0
        T1CON.TMR1ON = 0; // Para o temporizador TMR1
        INTCON.INT0IF = 0; // Zera a flag de interrup��o externa INTO
    }

    if (INTCON3.INT1IF) {
        // l�gica para o segundo bot�o (RB1)
        T1CON.TMR1ON = 1; // Inicia o temporizador TMR1
        T0CON.TMR0ON = 0; // Para o temporizador TMR0
        INTCON3.INT1IF = 0; // Zera a flag de interrup��o externa INT1
    }

    if (INTCON.TMR0IF) {
        // logica para quando ocorre a interrup��o do temporizador TMR0
        TMR0H = 0xC2; // Primeiro byte do TMR0
        TMR0L = 0xF7; // �ltimo byte do TMR0
        INTCON.TMR0IF = 0; // Zera a flag de interrup��o do temporizador TMR0
    }

    if (PIR1.TMR1IF) {
        // l�gica para a interrup��o do temporizador TMR1
        TMR1H = 0x0B; // Primeiro byte do TMR1
        TMR1L = 0xDC; // �ltimo byte do TMR1
        PIR1.TMR1IF = 0; // Zera a flag de interrup��o do temporizador TMR1
    }
    //Atualiza o display de 7 segmentos
        PORTD = display[contador];
        contador = (contador>= 9) ? 0 : contador + 1;
}

