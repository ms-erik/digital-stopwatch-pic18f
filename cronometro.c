int contador;
const int display[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

void main(){
    contador=0;

    TRISB.RB0 = 1; // Configura RB0 como um botão de entrada
    TRISB.RB1 = 1; // Configura RB1 como um botão de entrada
    TRISD = 0x00; // Configura RD como saída (display de 7 segmentos)
    PORTD = 0x00; // Configura a porta D como saída

    INTCON2.RBPU=0;

    RCON.IPEN = 1; // Habilida níveis de interrupção
    INTCON.GIEH = 1; // Chave geral de alta prioridade se IPEN = 1

    // Configuração da interrupção externa INTO
    INTCON.INT0IE = 1; // Habilita interrupção externa INT0
    INTCON.INT0IF = 0; // Flag para adicionar interrupção INT0
    INTCON2.INTEDG0 = 1; // Borda, 1 é a borda de subida ao soltar a tecla
                         // 0 é a borda de descida ao pressionar a tecla

    // Configuração do temporizador TWR0
    INTCON.TMR0IE = 1; // Habilita a interrupção associada ao temporizador TMR0
    INTCON.TMR0IF = 0; // Zera a flag.
    T0CON = 0b00000110; // Registro de controle do temporizador

    // Configuração do temporizador TMR1
    PIE1.TMR1IE = 1; // Habilita interrupção do temporizador TMR1
    PIR1.TMR1IF = 0; // Zera a flag de interrupção do temporizador TMR1
    IPR1.TMR1IP = 1; // Prioridade alta para TMR1
    T1CON = 0b00110000; // Configura o registrador T1CON, referente ao temporizador TMR1
                        // Prescaler 1:16, modo 16 bits

    // Overflow
    TMR1H = 0x0B; // Primeiro byte do TMR1
    TMR1L = 0xDC; // Último byte do TMR1

    TMR0H=0xC2;
    TMR0L=0xF7;


    while(1);
}

void INTERRUPTION_HIGH() iv 0x0008 ics ICS_AUTO {
    if (INTCON.INT0IF) { // se a interrupção amostrada foi a INT0
        // lógica para o primeiro botão (RB0)
        T0CON.TMR0ON = 1; // Inicia o timer TMR0
        T1CON.TMR1ON = 0; // Para o temporizador TMR1
        INTCON.INT0IF = 0; // Zera a flag de interrupção externa INTO
    }

    if (INTCON3.INT1IF) {
        // lógica para o segundo botão (RB1)
        T1CON.TMR1ON = 1; // Inicia o temporizador TMR1
        T0CON.TMR0ON = 0; // Para o temporizador TMR0
        INTCON3.INT1IF = 0; // Zera a flag de interrupção externa INT1
    }

    if (INTCON.TMR0IF) {
        // logica para quando ocorre a interrupção do temporizador TMR0
        TMR0H = 0xC2; // Primeiro byte do TMR0
        TMR0L = 0xF7; // Último byte do TMR0
        INTCON.TMR0IF = 0; // Zera a flag de interrupção do temporizador TMR0
    }

    if (PIR1.TMR1IF) {
        // lógica para a interrupção do temporizador TMR1
        TMR1H = 0x0B; // Primeiro byte do TMR1
        TMR1L = 0xDC; // Último byte do TMR1
        PIR1.TMR1IF = 0; // Zera a flag de interrupção do temporizador TMR1
    }
    //Atualiza o display de 7 segmentos
        PORTD = display[contador];
        contador = (contador>= 9) ? 0 : contador + 1;
}

