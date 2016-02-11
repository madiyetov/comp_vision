def primes(number):

    primes = []
    start = 2

    while start^2 <= n:

        if number%start == 0:
            primes.append(start)
            number //= start
        else:
            start += 1

    if number > 1:
        primes.append(number)

    return primes


if __name__ == '__main__':
    n = int(input('Enter number N: '))
    answers = [primes(number) for number in range(1, n+1)]

    for index, answer in enumerate(answers):
        print(index+1, ' -> ', answer)