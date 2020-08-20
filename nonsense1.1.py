from argparse import ArgumentParser
from collections import Counter
from xerox import copy
from random import choice
#parse args
p=ArgumentParser(description="Generate nonsense")
p.add_argument('--length',type=int,required=True,help='number of words in nonsense')
p.add_argument('--store',type=str,required=True,help='where to put the nonsense')
p.add_argument('--data',type=str,required=True,help='data to create nonsense')
p.add_argument('--seed',type=str,help='seed for algorithm, a word in data')
args=p.parse_args()
#refine data
with open(args.data,'r') as raw:words=raw.read().split(' ')
follow={}
already=set()
word1=words.pop(0)
for word2 in words:
	if word1 in already:
		follow[word1].update((word2,))
	else:
		follow[word1]=Counter((word2,))
		already.add(word1)
	word1=word2
for i in follow.keys():follow[i]=tuple(follow[i].elements())
#generate "nonsense"
word1=choice(tuple(follow.keys())) if args.seed==None else args.seed#seed
nonsense=word1
for _ in range(args.length-1):
	word2=choice(follow[word1])
	nonsense+=" "+word2
	word1=word2
if args.store=='clipboard':
	copy(nonsense)
else:
	with open(args.store,'w') as final:final.write(nonsense)
