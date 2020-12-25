 #include <stdio.h>

int main() {
    long long cardPublic;
    long long doorPublic;

    scanf("%lld %lld", &cardPublic, &doorPublic);

    long long calculated = 1;
    int i = 0;
    while (calculated != cardPublic) {
        calculated = (calculated * 7) % 20201227;
        i++;
    }

    calculated = 1;
    for (int j = 0; j < i; j++) {
        calculated *= doorPublic;
        calculated %= 20201227;
    }

    printf("%lld\n", calculated);
}
