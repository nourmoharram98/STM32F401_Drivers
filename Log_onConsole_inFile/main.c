// main.c

#include <stdio.h>
#include "math.h" // Including our math operations header
#include "logger.h" // Including the logger header

int main() {
    // Setting log priority to InfoPriority
    logger_init("log.txt");

    int num1, num2;
    printf("Enter two numbers: ");
    scanf("%d %d", &num1, &num2);

    // Log input values
    logger_log(LOG_INFO, "Input numbers: %d, %d\n", num1, num2);

    // Perform operations
    logger_log(LOG_INFO, "Sum: %d\n", add(num1, num2));
    logger_log(LOG_INFO, "Difference: %d\n", subtract(num1, num2));
    logger_log(LOG_INFO, "Product: %d\n", multiply(num1, num2));

    // Check if num2 is not zero before division to avoid division by zero error
    if (num2 != 0) {
        logger_log(LOG_INFO, "Quotient: %.2f\n", divide(num1, num2));
    } else {
        logger_log(LOG_ERROR, "Cannot divide by zero!\n");
    }

    logger_close();

    return 0;
}
