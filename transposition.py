import random, math

def encrypt(plain_text, key):
    
    plain_text = plain_text.lower()
    
    cipher_text = ""
    i = 0
    keymat = []
    temp = []
    
    for c in plain_text:
        temp.append(c)
        i+=1
        if(i==len(key)):
            keymat.append(temp)
            i=0
            temp=[]
    
    if(i!=lenkey and i>0):
        while(i!=lenkey):
            temp.append(' ')
            i+=1
        keymat.append(temp)
        
    for col in key:
        for row in range(len(keymat)):
            cipher_text += keymat[row][col]
    
    return cipher_text
    
    
def decrypt(cipher_text, key):
    cipher_text = cipher_text.lower()
    num_of_rows = int(math.ceil(len(cipher_text)/len(key)))
    
    plain_text = ""
    i = 0

    temp=[]
    for i in range(num_of_rows):
        for j in range(len(key)):
            pos=key.index(j)
            plain_text+=cipher_text[pos*num_of_rows+i]
    
    return plain_text


lenkey=int(input("\nEnter required length of the key (number of columns): "))
i = 0
key = []
while(i<lenkey):
    x=random.randrange(lenkey)
    if(x not in key):
        key.append(x)
        i+=1
print("\nRandom key is : ",key)

plaintext = input("Enter plaintext: ")

print("Encrypting: ")
print("Plaintext :", plaintext)
ciphertext = encrypt(plaintext, key)
ciphertext = encrypt(ciphertext, key)
print("Ciphertext :", ciphertext)

print("\n\nDecrypting: ")
print("Ciphertext :", ciphertext)
plaintext = decrypt(decrypt(ciphertext, key), key)
print("Plaintext :", plaintext)
