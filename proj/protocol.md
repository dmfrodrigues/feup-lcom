# Serial port protocol

Transmission/reception is processed at 38400 bps, with chars of 8 bits and even parity.

## Transmission

Transmission is performed on request by the program. It is done in a synchronous fashion. When starting a transmission, reception interrupt notifications are suspended. On ending, reception interrupt notifications are re-activated.

### Transmission of a char

1. 1 polls the readiness of THR in intervals of 50 microseconds, one at the beginning and up to 10 retries (up to 11 polling operations, and 500 microseconds). If after all the retries 1 has not yet sent the char, transmission is aborted.

2. If the char is transmitted, 1 expects 2 to answer the same char for confirmation. 1 polls the fullness of RBR in intervals of 50 microseconds, one at the beginning and up to 10 retries. If after all the retries 1 has not yet received the answer, transmission is aborted.

3. If 1 gets the answer, it compares it with the char it sent. If they are different, go back to step 2.

4. If they are equal, 1 sends OK symbol 0xFF using only step 1., and char transmission is complete.

### Transmission of a string

If the string has more than 1024 chars, transmission is not even started.

1. For each char in the string, transmit them, plus a start symbol 0x80 and an end symbol 0xEF. If transmission fails for a given char, transmission of the string fails, and none of the following chars in the same transmission request are transmitted.

2. If all symbols are transmitted correctly, transmission is successfully terminated.

## Reception

Reception is made through interrupts. On a reception interrupt, 2 must handle the interrupt. On a reception interrupt, reception interrupt notifications are suspended, and on ending a reception notifications are re-activated.

### Reception of a char

1. Upon getting a reception interrupt, 2 grabs the char, and then polls the readiness of the THR in intervals of 50 microseconds, one at the beginning and up to 10 retries. If after 10 retries 2 has not sent the char, transmission is aborted.

2. If transmission is successful, 2 expects 1 to answer the OK symbol 0xFF, using only step 1. to get the OK. If the received symbol has less or equal to 4 bits set to 1, transmission is aborted.

3. If the received OK has at least 4 bits set to 1 then it is accepted.

### Reception of a string

1. 2 expects the first char to be the start char. If it is not, transmission is aborted.

2. If the first char is correct, all chars are received. If one of the char receptions fails (timeout), reception fails. If after receiving 1024 chars the end symbol is not received, transmission is aborted.

3. If all receptions are successful and the end symbol is received, transmission is successful.
