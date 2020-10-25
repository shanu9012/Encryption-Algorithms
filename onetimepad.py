import random
    
def encrypt(plain_text, key):
    
    plain_text = plain_text.upper()

    cipher_text = ""
    for i in range(len(plain_text)):
        if(plain_text[i] == ' '):
            cipher_text += ' ' 
        else:
            cipher_text += chr(((ord(plain_text[i])-65) + (ord(key[i]) - 65)) % 26 + 65)
        i+=1
    
    return cipher_text
    
    
def decrypt(cipher_text, key):
    
    plain_text = ""
    
    for i in range(len(cipher_text)):
        if(cipher_text[i] == ' '):
            plain_text += ' '
        else:
            plain_text += chr((ord(cipher_text[i])-ord(key[i])) % 26 + 65)
    
    return plain_text.lower()


print("One-time Pad Cipher\n")

message = input("Enter message : ")

key = ""
for l in range(len(message)):
    key += chr(random.randint(65,90))
print("Random key is :", key)

print("\nEncrypting: \n" + "Message: " + message)
print("Cipher text: ", end='') 
cipher_text = encrypt(message, key)
print(cipher_text)

print("\nDecrypting: \n" + "Cipher: " + cipher_text)
print("Plaintext: ", end='')
print(decrypt(cipher_text, key))