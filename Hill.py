KeyMatrix = []
KeyMatrix.append([6, 24, 1])
KeyMatrix.append([13, 16, 10])
KeyMatrix.append([20, 17, 15])

InvertMatrix = []
InvertMatrix.append([8, 5, 10])
InvertMatrix.append([21, 8, 21])
InvertMatrix.append([21, 12, 8])


def multiply(a, mode):
	res = []
	res.append([0])
	res.append([0])
	res.append([0])
	for i in range(3):
		for j in range(1):
			for k in range(3):
				if(mode == "encrypt"):
					res[i][j] += KeyMatrix[i][k] * a[k][j]
				elif(mode == "decrypt"):
					res[i][j] += InvertMatrix[i][k] * a[k][j]				
	for i in range(3):
		res[i][0] %= 26
	return res
	

def encrypt(plaintext):
	ciphertext = ""
	i = 0
	a = ['','','']
	
	while(i<len(plaintext)):
		a[0] = ord(plaintext[i])-97
		a[1] = ord(plaintext[i+1])-97
		a[2] = ord(plaintext[i+2])-97
		
		mat = [[a[0]], [a[1]], [a[2]]]	
		mat = multiply(mat, "encrypt")
		
		ciphertext += chr(mat[0][0]+97)
		ciphertext += chr(mat[1][0]+97)
		ciphertext += chr(mat[2][0]+97)
		
		i += 3
	
	return ciphertext


def decrypt(ciphertext):
	plaintext = ""
	i = 0
	a = ['','','']
	
	while(i<len(ciphertext)):
		a[0] = ord(ciphertext[i])-97
		a[1] = ord(ciphertext[i+1])-97
		a[2] = ord(ciphertext[i+2])-97
		
		mat = [[a[0]], [a[1]], [a[2]]]	
		mat = multiply(mat, "decrypt")
		
		plaintext += chr(mat[0][0]+97)
		plaintext += chr(mat[1][0]+97)
		plaintext += chr(mat[2][0]+97)
		
		i += 3

	return plaintext


plaintext = input("Enter plaintext (Length should be multiple of 3) : ")
if(len(plaintext) % 3 != 0):
	print("Wrong Input")
else:
	print("Encrypting: ")
	print("Plaintext :", plaintext)
	ciphertext = encrypt(plaintext)
	print("Ciphertext :", ciphertext)
	
	print("\nDecrypting: ")
	print("Ciphertext :", ciphertext)
	plaintext = decrypt(ciphertext)
	print("Plaintext :", plaintext)