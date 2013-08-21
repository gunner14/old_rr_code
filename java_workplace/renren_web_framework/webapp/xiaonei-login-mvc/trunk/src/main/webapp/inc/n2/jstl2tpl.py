#coding: utf-8
#!/usr/bin/env python

import os, sys

"""
<str 	{{:u
<c:if 	{{#
<c:out   	{{}}
<c:choose
  <c:when
  <c:otherwise

<c:if test="${visiter!=null && visiter.id%10==3 && 
  !visiter.userStateInfo.guideDone && visiter.userStateInfo.socialGraphGood}">

<c:forEach var items, begin, end>

${requestScope.postFormSession}


<c:set  	c++ code
"""

class Converter():
  def Ignore(self, line):
    # ignore <%@ <%--
    IGNORE_ARR = ('<%@')
    for i in IGNORE_ARR:
      if line.startswith(i):
        return True
    return False
  
  def ProcessBlock(self, idx, arr, p=None):
    r = repr(arr)
    if len(r) > 60:
      print idx, r[:30], '...', r[len(r)-30:]
    else:
      print idx, r
    if len(p) > 2:
      p[2](idx, arr)
      
  def ProcessComment(self, idx, arr):
    pass
    
  def ProcessIf(self, idx, arr):
    pass
    
  def ProcessSet(self, idx, arr):
    pass
    
  def ProcessValue(self, idx, arr):
    key = arr[0]
    key = key.strip('${\'}')
    key = key.replace('.', '_')
    #TODO: if key.find('[') != -1:
    print >>self._src, 'define %s' % key.upper()
    self._tpl.write('{{%s}}' % key.upper())
    
  def Process(self, idx, line, e):
    line = line.rstrip('\n\r\f')

    #if self.Ignore(line):
    #  continue

    BLOCK = [('<%--', '--%>', self.ProcessComment),
             #
             ('<c:if', '</c:if>', self.ProcessIf),
             ('<c:set ', '/>', self.ProcessSet),
             ('${', '}', self.ProcessValue),
             ('<%@', '%>', self.ProcessComment),
             ]
    arr = []
    first = True
    for p in BLOCK:
      start = line.find(p[0])
      if -1 != start:
        if start > 0 and first:
          self._tpl.write(line[:start])
          first = False
        end = line.find(p[1], start)
        if -1 != end:
          arr.append(line[start:end + len(p[1])])
          self.ProcessBlock(idx, arr, p)
          arr = []
          if end + len(p[1]) != len(line):
            line = line[end + len(p[1]):]
            self.Process(idx, line, e)
            print 'how here', idx, line
          break
        else:
          arr.append(line[start:])
          while True:
            idx2, line = e.next()
            line = line.rstrip('\n\r\f')
            end = line.find(p[1])
            # print 'while(true)', idx2, line, end
            if -1 == end:
              arr.append(line)
            else:
              arr.append(line[:end + len(p[1])])
              self.ProcessBlock(idx, arr, p)
              line = line[end + len(p[1]):]
              arr = []
              break
    else:
      #if line:
      print >>self._tpl, line
        
  def __init__(self, inf, tpl, src):
    #  inf.seek(0,  os.SEEK_END)
    #  size = inf.tell()
    #  inf.seek(0,  os.SEEK_SET)
    #  buf = inf.read(size)
    # lines = inf.readlines()
    self._tpl = tpl
    self._src = src
    e = enumerate(inf)
    for idx, line in e:
      self.Process(idx, line, e)

def main():
  filename = sys.argv[1]
  basename = os.path.splitext(os.path.basename(filename))[0]
  tpl = '%s.tpl' % basename
  src = '%s.cc' % basename
  Converter(open(filename, 'rb'), open(tpl, 'w+b'), open(src, 'w+b'))

if __name__ == '__main__':
  main()
