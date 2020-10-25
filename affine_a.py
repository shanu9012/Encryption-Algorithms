from fractions import gcd

m=26
a=2
b=7

def setup():
    global m,a,b
    b=int(input("\nEnter number B : "))
    while True:
        a=int(input("\nEnter number A relatively prime to 26 : "))
        if(a==1 and b==0):
            continue
        if(gcd(a,m)==1):
            break
    

def inverse(a):
    for i in range(m):
        if((a*i)%m==1):
            return(i)


def encrypt():
    global m,a,b
    st=""
    message=input("\nEnter message to be encrypted : ")
    message=message.lower()
    for i in list(message):
        if(i==' '):
            st+=i
        else:
            st+=chr((a*(ord(i)-97)+b)%m+97)
    print("\nEncrypted text is : ",st)
    
def decrypt():
    global m,a,b
    inva=inverse(a)
    cipher=input("\nEnter message to be decrypted : ")
    st=""
    cipher=cipher.lower()
    for i in list(cipher):
        if(i==' '):
            st+=i
        else:
            st+=chr(inva*(ord(i)-97-b)%m+97)
    print("\nDecrypted text is : ",st)


choice=1
print("------------------------------------- Affine Cipher -----------------------------------")

while(choice==1 or choice==2):
    choice=int(input("\n\nPress and Enter \n1.To encrypt \n2.To decrypt \nPress anything else to exit  : "))
    if(choice==1):
        setup()
        encrypt()
    elif(choice==2):
        setup()
        decrypt()
    else:
        break
