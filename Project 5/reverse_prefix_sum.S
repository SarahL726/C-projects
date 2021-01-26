;;; Xinyue Li
;;; 116560740
;;; sarahl

;;; Global data
        .data
pctd1:
          .asciz "%d"
pctd2:
          .asciz " %d"

        .text

;;; Reverse prefix sum

        .global reverse_prefix_sum
reverse_prefix_sum:
        ;; Transform an array by adding the value at an index to the su$
        movw r26, r24
        clr r19
        clr r24
        clr r25
        ld r18, X+
        cpi r18, -1
        breq 1f

        push r30
        push r31
        push r18
        movw r24, r26
        call reverse_prefix_sum         ; recursive call
        pop r18
        pop r31
        pop r30

        add r24, r18
        adc r25, r19
        st -X, r24

1:
      ret

    .global print_array
print_array:
    movw r26, r24
        clr r25
        ld r24, X+
        cpi r24, -1
        breq 2f

        ldi r22, lo8(pctd1)
        ldi r23, hi8(pctd1)
        call pint
        ldi r22, lo8(pctd2)
        ldi r23, hi8(pctd2)
        jmp 1f

1:
        ld r24, X+
        cpi r24, -1
        breq 2f

        call pint
        jmp 1b

2:
      ldi r24, 10
        clr r25
        call putchar
        ret

pint:
         push r27
        push r26
        push r25
        push r24
        push r23
        push r22
        call printf
        pop r22
        pop r23
        pop r24
        pop r25
        pop r26
        pop r27
        ret

