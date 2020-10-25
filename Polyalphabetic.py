def encrypt(plaintext, key):
	ciphertext=""
	count = 0
	
	for i in range(len(plaintext)):
		p = ord(plaintext[i])-97
		q = ord(key[count])-97
		p += q
		p %= 26
		ciphertext += chr(p+97)
		count += 1
		count %= len(key)
	
	return ciphertext


def decrypt(ciphertext, key):
	plaintext=""
	count=0
	
	for i in range(len(ciphertext)):
		p = ord(ciphertext[i])-97
		q = ord(key[count])-97
		p -= q
		
		if(p>0):
			p %= 26
		else:
			while(p<0):
				p += 26
		
		plaintext += chr(p+97)
		count += 1
		count %= len(key)

	return plaintext


plaintext = input("Enter plaintext: ")
key = input("Enter key: ")

print("Encrypting: ")
print("Plaintext :", plaintext)
ciphertext = encrypt(plaintext, key)
print("Ciphertext :", ciphertext)

print("\n\nDecrypting: ")
print("Ciphertext :", ciphertext)
plaintext = decrypt(ciphertext, key)
print("Plaintext :", plaintext)