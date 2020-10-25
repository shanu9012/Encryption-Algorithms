import math

def inverse(a,m):
	for i in range(1,m):
		if((a*i)%m==1):
			return i

def encrypt(a, b, plaintext):
	ciphertext = ""
	l=len(plaintext)
	
	for i in range(l):
		x=ord(plaintext[i])-97
		val=(a*x)
		val+=b
		val%=26
		ciphertext += chr(val+97)
	
	return ciphertext
	
	
def decrypt(a, b, ciphertext):
	plaintext = ""
	l=len(ciphertext)
	
	a=inverse(a,m)
	
	for i in range(l):
		x = ord(ciphertext[i])-97
		val = (x-b)
		val *= a
		if(val>=0):
			val %= 26
		else:
			while(val<0):
				val += 26
		plaintext += chr(val+97)
	
	return plaintext


a=int(input("Enter a (must be coprime with 26) : "))
m = 26

if(math.gcd(a,26)!=1):
	print("a is not coprime with 26.")
else:
	b = int(input("Enter b: "))
	plaintext = input("Enter plaintext: ")
	
	print("Encrypting: ")
	print("Plaintext :", plaintext)
	ciphertext = encrypt(a, b, plaintext)
	print("Ciphertext :", ciphertext)
	
	print("\n\nDecrypting: ")
	print("Ciphertext :", ciphertext)
	plaintext = decrypt(a, b, ciphertext)
	print("Plaintext :", plaintext)