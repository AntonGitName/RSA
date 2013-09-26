import random

test_file = open("test.txt", "w")
answer_file = open("answer.txt", "w")
l, r = -2**512, 2**512
for i in range(1000):
	a = random.randint(l, r)
	b = random.randint(l, r)
	c = a * b
	test_file.write('{0} {1}\n'.format(a, b))
	answer_file.write('{0}\n'.format(c))
test_file.close()
answer_file.close()