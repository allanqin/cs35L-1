#!/usr/bin/python

import sys

def DataTypes(dummy):
    #Integers & Floats
    tempInt = 5
    tempFloat = 4.5 

    #Strings
    sampleSTR = "My first python script!"
    print sampleSTR
    sampleSTR = sampleSTR + 'A' + sampleSTR * 2
    # + for joining strings and * repeating
    sampleSTR[0]
    sampleSTR[5]
    len(sampleSTR)
    sampleSTR[3:7]
    sampleSTR[3:len(sampleSTR)]
    sampleSTR[:5]
    sampleSTR[5:]
    sampleSTR[-1]
    #Immutable!Cannot change its members
    #sample[5] = 'd'

    #Lists are mutable
    lst = [10, 20, 'hello', 'world']
    lst[0]
    lst[3]
    lst[:]
    lst[2] = 30
    lst.append('hello')
    lst[:] = []

    
    
    #Tuples are immutable
    #cannot assign values like tup[0]=6
    tup = (1,2,3,4)

    
    
    #Sets - No duplicate elements. Good for membership testing and finding duplicates
    s1 = set('abracadabra')
    s2 = set('alacazam')
    print 'The sets contain - ', s1, s2
    #Supports set operations
    s1 - s2 #elements in s1 but not in s2
    s1 | s2 # OR operation
    s1 & s2 # AND operation
    s1 ^ s2 # XOR operation

    #Converting a list to a set with unique elements, and back to a list again
    basket = ['apple', 'orange', 'apple', 'pear', 'orange', 'banana']
    uniquebasket = list(set(basket))


    
    #Dictionary: Associative arrays. for having (key,value) pairs. Keys are unique.
    d = {}
    d["Tom"] = 12345
    d["SeasHelp"] = 67899
    #Unique keys
    d['Tom'] = 911
    d.values()
    d.keys()
    d.items()
    print d
    if 'Tom' in d.keys():
        print 'Tom Present!'
    elif 'SeasHelp' in d.keys():
        print 'Seas present!'
    else:
        print 'None'



def Looping(forloops, whileloops):
    a = 0
    while a < 10:
        print 'The current value of a is', a
        print 'The current value of a is {0} and a+1 is {1}'.format(a, a+1)
        a = a + 1

    for i in range(0, 10):
        print 'The current value of i is', i

    lst = ['a', 'b', 'c', 'd']
    for i in range(len(lst)):
        print i, lst[i]

    range(3, 10)
    #Generates a list [3, 4, 5, 6, 7, 8, 9]
    #Increments of 2
    range(0, 10, 2)
    #Generates list [0, 2, 4, 6, 8]

    return lst


class Engine:
    modelID=0
	#Constructor with a reference to self - Like the "this" pointer in c++
    def __init__(self, ID):
        self.modelID = ID

    def printID(self):
        print 'Engine model ID is ', self.modelID




def main():
	#sys.argv for accessing command line arguments
    print sys.argv
    Looping(5, 5)
    DataTypes([5, 4, 5, 'arguments'])
    myEngine = Engine(20)
    myEngine.printID()
    print 'You can access the internal members directly. modelID is ', myEngine.modelID

#Defining an entry point for the script
if __name__=="__main__":
    main()

