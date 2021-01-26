;;; Xinyue Li
;;; 116560740
;;; sarahl

;;; Implementation that computes the nth fibonacci number.

    .global fib                 ; exposes fib as a label for the linker
fib:                            ; the label: the next instruction starts fib()
    ;; recursive computation of fibonacci
    cpi r24, 0                  ; base case check (if value == 0)
    breq 1f
    cpi r24, 1                  ; base case check (if value == 1)
    breq 1f

    push r24
    push r25
    sbiw r24, 1
    call fib                    ; recursive call
    pop r31
    pop r30
    sbiw r30, 2
    push r24
    push r25
    movw r24, r30               ; copies r31:r30 to r25:r24
    call fib                    ; recursive call
    pop r31
    pop r30
    add r24, r30
    adc r25, r31

1:
    ret                         ; returns.  necessary.
