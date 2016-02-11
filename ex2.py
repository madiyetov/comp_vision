def divide(n, m):

    intervals = []
    element_amount = n / m
    rest_part = n % m
    
    start = rest_part / 2
    end = n - (rest_part - rest_part / 2)

    for i in range(start, end, element_amount):
        intervals.append((i, i + (element_amount-1)))

    return intervals


if __name__ == '__main__':
    
    size_of_list = int(input("Enter the size of list: "))
    parts = int(input("Enter the amount of parts: "))
    
    print divide(size_of_list, parts)
