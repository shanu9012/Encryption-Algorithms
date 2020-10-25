plain_text = input("Enter plain text: ").lower()

key = input("Enter key: ")

cipher_text = ""

for c in plain_text:
	if(c == " "):
		cipher_text += " "
	else:
		cipher_text += chr(((ord(c)+int(key)-97) % 26) + 97)

print("Cipher text is : ", end="")
print(cipher_text)

recovered_plain_text = ""

for c in cipher_text:
	if(c == " "):
		recovered_plain_text += " "
	else:
		recovered_plain_text += chr(((ord(c)-int(key)-97) % 26) + 97)

print("Recovered plain text: ", end="")
print(recovered_plain_text)
