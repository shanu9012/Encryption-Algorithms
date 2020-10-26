def encrypt(plain_text, key):
	cipher_text = ""
	for c in plain_text:
		if(c == " "):
			cipher_text += " "
		else:
			cipher_text += chr(((ord(c)+int(key)-97) % 26) + 97)
	return cipher_text

def decrypt(cipher_text, key):
	recovered_plain_text = ""
	for c in cipher_text:
		if(c == " "):
			recovered_plain_text += " "
		else:
			recovered_plain_text += chr(((ord(c)-int(key)-97) % 26) + 97)
	return recovered_plain_text

def main(): 
	plain_text = input("Enter plain text: ").lower()
	key = input("Enter key: ")
	cipher_text = encrypt(plain_text, key)
	print("Cipher text is : ", end="")
	print(cipher_text)
	
	print("Recovered plain text: ", end="")
	print(decrypt(cipher_text, key))

if __name__=="__main__": 
	main()
