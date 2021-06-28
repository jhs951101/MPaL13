void boardSet() // 조이스틱 내의 인터럽트를 세팅하는 함수
{

 rRCC_APB2ENR = rRCC_APB2ENR | (1<<5) | (1<<6);

 *(unsigned int *) 0xE000E100 |= (0x1 << 6);

 *(unsigned int *) 0xE000E100 |= (0x1 << 7);

 *(unsigned int *) 0xE000E100 |= (0x1 << 23);

 *(unsigned int *) 0xE000E104 |= (0x1 << 8);

 rEXTI_IMR = rEXTI_IMR | (1<<12) | (1<<14) | (1<<1) | (1<<0) | (1<<8);

 rEXTI_FTSR = rEXTI_FTSR | (1<<12) | (1<<14) | (1<<1) | (1<<0) | (1<<8);

 rAFIO_EXTICR4 = rAFIO_EXTICR4 & ~(0xf << 0);
 rAFIO_EXTICR4 = rAFIO_EXTICR4 | (0x3 << 0);
 rAFIO_EXTICR4 = rAFIO_EXTICR4 & ~(0xf << 8);
 rAFIO_EXTICR4 = rAFIO_EXTICR4 | (0x3 << 8);

 rAFIO_EXTICR1 = rAFIO_EXTICR1 & ~(0xf << 4);
 rAFIO_EXTICR1 = rAFIO_EXTICR1 | (0x4 << 4);

 rAFIO_EXTICR1 = rAFIO_EXTICR1 & ~(0xf << 0);
 rAFIO_EXTICR1 = rAFIO_EXTICR1 | (0x4 << 0);
 rAFIO_EXTICR3 = rAFIO_EXTICR3 & ~(0xf << 0);
 rAFIO_EXTICR3 = rAFIO_EXTICR3 | (0x3 << 0);

 LCD_Setup();
 STM3210B_LCD_Init();
}
int i = 0; // i: i 값을 이용해 2 차원 배열 안에서 좌표값을 끌어들일 수 있음
int beforeI = 0; // beforeI: 이전의 i 값을 저장해놓는 변수
int lcdsign[4][2] = {{319, 0}, {150, 0}, {319, 120}, {150, 120}};
// lcdsign: 4 개의 이미지의 각 좌표 값을 저장해놓는 2 차원 배열
/*
2 차원 배열 {A, B, C, D} 를 화면에 표현했을 때의 모습 (A, B, C, D 는 x 와 y 의 값을 담고 있음)
A B
C D
*/
// ex) A = (12, 25) : A 위치의 x 좌표는 12, y 좌표는 25
void initImage(){ // LCD 를 검은색으로 Clear 하고 이미지 4 개를 처음 상태로 세팅해주는 함수
 LCD_Clear(Black);

 for(int x=0; x<4; x++)
 LCD_DrawArray(btn, lcdsign[x][1], lcdsign[x][0], 120, 102);
}
void outBeforePos(){ // 이전의 위치에 이미지를 덮어 씌워주는 함수
 LCD_DrawArray(btn, lcdsign[beforeI][1], lcdsign[beforeI][0], 120, 102);
}
void selectImage(){

 boardSet();

 initImage();

 while(1)
 {
 unsigned int act = 0; // act: 조이스틱 버튼이 눌렸을 때 예외적인 상황만 아니면 1 이 되어
밑의 if 문이 실행됨

 if(gExti[0]) // right key is pressed
 {
 if(i % 2 == 0){ // 현재 좌측이 선택된 상태라면
 outBeforePos();
 i += 1; // i 값을 1 만큼 증가시킴으로써 우측으로 이동시킬 수 있음
 act = 1; // if 문을 어느 한 곳이라도 통과하면 1 이 되어 밑의 if 문이 실행됨
 }

 gExti[0] = 0;
 }
 else if(gExti[1]) // left key is pressed
 {
 if(i % 2 == 1){ // 현재 우측이 선택된 상태라면
 outBeforePos();
 i -= 1;
 act = 1;
 }
 
 gExti[1] = 0;
 }
 else if(gExti[8]) // up key is pressed
 {
 if(i == 2 || i == 3){ // 현재 하단이 선택된 상태라면
 outBeforePos();
 i -= 2;
 act = 1;
 }

 gExti[8] = 0;
 }
 else if(gExti[14]) // down key is pressed
 {
 if(i == 0 || i == 1){ // 현재 상단이 선택된 상태라면
 outBeforePos();
 i += 2;
 act = 1;
 }

 gExti[14] = 0;
 }

 if(act == 1){ // 선택된 위치에 그림을 덮어씌워준 다음 이전의 i 값을 세팅해줌
 LCD_DrawArray(btnpressed, lcdsign[i][1], lcdsign[i][0], 120, 102);
 beforeI = i;
 }

 if(Uart_GetKey())
 break;
 }
}
