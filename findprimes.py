#    if (1 >= check)
#    {
#        return false;
#    }
#
#    if (3 >= check)
#    {
#        return true;
#    }
#
#    if ((0 == check % 2) || (0 == check % 3))
#    {
#        return false;
#    }
#
#    for (size_t factor = 5; factor * factor <= check; factor = factor + 6)
#    {
#        if ((0 == check % factor) || (0 == check % (factor + 2)))
#        {
#            return false;
#        }
#    }

for i in range (2, 100):
    print(i, end=" ")
print("")

for i in range (2, 100):
    if i % 2 != 0:
        print(i, end=" ")
print("")

for i in range (2, 100):
    if i % 2 != 0 and i %3 != 0:
        print(i, end=" ")
print("")

# for i in range (2, 100):
#     prime = True
#     for j in range(2, i - 1):
#         if 0 == i % j:
#             prime = False
#             break
#     if prime is True:
#         print(i)

