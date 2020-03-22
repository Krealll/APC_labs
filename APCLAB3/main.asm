org 100h

.model small

.stack 100h

.data

msgHello db "1 - start resident",10,13,"$"
buffer db "IRQ:    "            ;Буфер для заголовка


temp db 8 dup ('6')             ;Буфер для обработки и вывода значения регистра

registers db 6 dup (?)          ;Массив регистров

BLACK equ 01110000b             ;Атрибут текста - черные буквы на белом фоне

RED equ 01110100b               ;Атрибут текста - красные буквы на белом фоне
errMsg db "Error. Try again",10,13,"$"
.386
.code
start:
    mov ax,@data
    mov ds,ax

jmp input
beg:
    lea dx, errMsg
    mov ah,09h
    int 21h
input:   
    mov ah,09h
    lea dx,msgHello
    int 21h

    mov ah,07h
    int 21h

    cmp al,'1'
    jne beg
    je setInterrupts

IRQ0:                       ;Обработчик IRQ0 - прерывание системного таймера

    int 50h                 ; Прерывание для вывода значений регистров на экран

    int 08h

    iret

IRQ1:                       ;Обработчик IRQ1 - прерывание клавиатуры

    int 50h

    int 09h
    
    iret

IRQ2:                       ;Обработчик IRQ2 - прерывание для ведомого контроллера

    int 50h

    int 0Ah

    iret

IRQ3:                       ;Обработчик IRQ3 - прерывание COM2

    int 50h

    int 0Bh
    
    iret

IRQ4:                       ;Обработчик IRQ4 - прерывание COM1

    int 50h

    int 0Ch

    iret

IRQ5:                       ;Обработчик IRQ5 - прерывание контроллера жесткого диска

    int 50h

    int 0Dh

    iret

IRQ6:                       ;Обработчик IRQ6 - прерывание контроллера флоппи-диска

    int 50h

    int 0Eh

    iret

IRQ7:                       ;Обработчик IRQ7 - прерывание принтера

    int 50h

    int 0Fh

    iret

IRQ8:                       ;Обработчик IRQ8 - прерывание RTC

    int 50h

    int 70h

    iret

IRQ9:                       ;Обработчик IRQ9 - прерывание контроллера EGA

    int 50h

    int 71h

    iret

IRQ10:                      ;Обработчик IRQ10 - зарезервировано

    int 50h

    int 72h

    iret

IRQ11:                      ;Обработчик IRQ11 - зарезервировано

    int 50h

    int 73h

    iret

IRQ12:                      ;Обработчик IRQ12 - зарезервировано

    int 50h

    int 74h

    iret

IRQ13:                      ;Обработчик IRQ13 - прерывание математического сопроцессора

    int 50h

    int 75h

    iret

IRQ14:                      ;Обработчик IRQ14 - прерывание контроллера жесткого диска

    int 50h

    int 76h

    iret

IRQ15:                      ;Обработчик IRQ15 - зарезервировано

    int 50h

    int 77h

    iret

display:                    ;Обработчик INT50 - вывод значения регистров контроллера прерываний

    pusha

    push es 

    mov ax,0B800h

    mov es,ax               ; Помещаем адрес видеопамяти

    mov cx,8

    xor di,di

    mov di,0

    xor bx,bx 

bufferOutput:

    mov al,cs:[buffer+bx]

    mov es:[di],al

    inc bx

    inc di

    mov al,BLACK

    mov es:[di],al

    inc di

loop bufferOutput

    in al,21h                       ;Получение значения регистра масок ведущего контроллера
    mov cs:[registers+0],al

    in al,0A1h                      ;Получение значения регистра масок ведомого контроллера
    mov cs:[registers+3],al


    mov al,00001011b ; OCW3 ..11- считать регистр обслуживания


    out 20h,al
    in al,20h
    mov cs:[registers+2],al


    mov al,00001011b ;OCW3 


    out 0A0h,al
    in al,0A0h
    mov cs:[registers+5],al

    mov al,00001010b ; OCW3 ..10 - считать регистр запросов

    out 20h,al
    in al,20h
    mov cs:[registers+1],al

    mov al,00001010b ; OCW3

    out 0A0h,al

    in al,0A0h

    mov cs:[registers+4],al

    mov cx,6 
registersOutput:

    mov dx,cx

    mov bx,6

    sub bx,dx

    mov al,cs:[registers+bx] 

    lea bx,cs:[temp]

    push cx

    mov cx,8

btbs_lp: ;Перевод числа в двоичный вид

    rol al,1 

    jc btbs_1 ;Если выдвинутый бит = 1, то переход

    mov cs:[bx],'0' ;Добавление символа '0' в строку

    jmp btbs_end

btbs_1:

    mov cs:[bx],'1' ;Добавление символа '1' в строку

btbs_end:

    inc bx 

    loop btbs_lp 

    mov cx,8

    xor bx,bx

registerOutput: ;Цикл по выводу буфера

    mov al,cs:[temp+bx]

    mov es:[di],al

    inc di

    inc bx

    mov al,RED

    mov es:[di],al

    inc di

loop registerOutput

    mov al,' ' ;Добавить пробел после буфера

    mov es:[di],al

    inc di

    mov al,RED

    mov es:[di],al

    inc di

    pop cx

loop registersOutput

    pop es

    popa

    iret

setInterrupts: ;Процедура установки обработчиков аппаратных прерываний и переинициализации контроллера

    mov ah,0h ;Установка видеорежима

    mov al,3h ;Консоль, 16 цветов, 80*25 символов,

    int 10h ;Установить видео режим

    push ds

    push cs

    pop ds

    mov ax,25A0h                ;Инициализация векторов прерываний   в al - номер прерывания

    mov dx, offset IRQ0

    int 21h

    mov ax,25A1h

    mov dx, offset IRQ1

    int 21h

    mov ax,25A2h

    mov dx, offset IRQ2

    int 21h

    mov ax,25A3h

    mov dx, offset IRQ3

    int 21h

    mov ax,25A4h

    mov dx, offset IRQ4

    int 21h

    mov ax,25A5h

    mov dx, offset IRQ5

    int 21h

    mov ax,25A6h

    mov dx, offset IRQ6

    int 21h

    mov ax,25A7h

    mov dx, offset IRQ7

    int 21h

    mov ax,25A8h

    mov dx, offset IRQ8

    int 21h

    mov ax,25A9h

    mov dx, offset IRQ9

    int 21h

    mov ax,25AAh

    mov dx, offset IRQ10

    int 21h

    mov ax,25ABh

    mov dx, offset IRQ11

    int 21h

    mov ax,25ACh

    mov dx, offset IRQ12

    int 21h

    mov ax,25ADh

    mov dx, offset IRQ13

    int 21h

    mov ax,25AEh

    mov dx, offset IRQ14

    int 21h

    mov ax,25AFh

    mov dx, offset IRQ15

    int 21h

    mov ax,2550h

    mov dx, offset display

    int 21h

    cli         ;Запрет аппаратных прерываний

    mov al, 11h ;ICW1 - начало инициализации контроллеров

    out 20h, al ;Инициализация ведущего контроллера

    out 0A0h, al ;Инициализация ведомого контроллера

    mov al, 0A0h ;ICW2 - базовые номера вектора прерывания ведущего контроллера

    out 21h, al

    mov al, 0A8h ;ICW2 - базовые номера вектора прерывания ведомого контроллера

    out 0A1h, al

    mov al, 04h ;ICW3 - ведущий контроллер подключен к 3 линии

    out 21h, al

    mov al, 02h ;ICW3 - ведомый контроллер подключен к 3 линии

    out 0A1h, al

    mov al, 11h ;ICW4 - режим специальной полной вложенности для ведущего контроллера

    out 21h, al

    mov al,01h ;ICW4 - режим обычной полной вложенности для ведомого контроллера

    out 0A1h, al
    
    in al,0A1h
    or al,00010000b
    out 0A1h,al

    sti

    lea dx,setInterrupts    

    int 27h

    ret

end start