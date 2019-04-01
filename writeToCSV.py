import sys
import pandas as pd
import numpy as np
import wikipedia as wp
from wikipedia.exceptions import DisambiguationError, PageError

# generates all potential children
def get_initial(topic):
    all = ""
    topic_page = wp.page(topic)
    for link in topic_page.links:
        if link.lower() != topic.lower() and topic_page.summary.lower().count(link.lower()) > 1:
            all += (link + '.' + '\n')
    return all

#returns 1 if topic1 is parent of topic2, followed by the strength of the relationship
def parent_or_child(topic1, topic2, cached_page = None): #returns 1 if topic1 is parent of topic2
    topic1 = topic1.lower()
    topic2 = topic2.lower()
    try:
        topic1_content = wp.page(topic1).content.lower() if cached_page == None else cached_page.content.lower()
        topic2_content = wp.page(topic2).content.lower()
        count1in2 = topic2_content.count(topic1) / len(topic2_content) if len(topic2_content) > 0 else 0
        count2in1 = topic1_content.count(topic2) / len(topic1_content)
        if count2in1 == 0: #no topic2 reference detected in topic1's content
        # thus labelled as a sibling relation
            pAndC = (str(0) + '.' + '\n')
            pAndC += (str(1) + '.' + '\n')
        elif count1in2 > count2in1: # more references
            pAndC = (str(1) + '.' + '\n')
            pAndC += (str((count1in2 - count2in1) / count1in2) + '.' + '\n')
        else:
            pAndC = (str(-1) + '.' + '\n')
            pAndC += (str((count2in1 - count1in2) / count2in1) + '.' + '\n')
    except DisambiguationError or PageError: #title did not match to page
        pAndC = (str(-2) + '.' + '\n')
        pAndC += (str(1) + '.' + '\n')
    return pAndC

#since a space in the argv is confused with a new argument, phrases are passed in with
#a comma delimiter, so this reverses all commas with spaces
for i in range(len(sys.argv)):
    sys.argv[i] = ' '.join(sys.argv[i].split(','))

if len(sys.argv) == 3:
    resultString = parent_or_child(sys.argv[1], sys.argv[2])
elif len(sys.argv) == 2:
    resultString = get_initial(sys.argv[1])

# makes sure there's no special symbols which can't be read into C++
resultString = ''.join(i for i in resultString if ord(i) < 128)
if len(resultString) > 0:
    print(resultString[:-1] if resultString[-1] == '\n' else resultString)
#because the last char will be a new line
