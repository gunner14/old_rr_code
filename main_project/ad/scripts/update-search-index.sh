  echo "Generating content data on server ..."
  ssh www@jz "cd ~/python && sh ./load-article-content.sh && rm -fv content.tgz && tar -zcvf content.tgz articles-body" 
  appendix=$(date +%s)

  mv -fv x-article-titles.txt x-article-titles.txt.${appendix}
  echo "Copy id & title from server ..."
  scp www@jz:~/python/x-article-titles.txt .

  # 备份、清理老数据
  if [ -f content.tgz ]; then
    mv -f content.tgz content.${appendix}.tgz
  fi
  if [ -f articles-body ]; then
    rm -fr articles-body articles-body.${appendix} 
  fi

  echo "Copy content from server ..."
  scp www@jz:~/python/content.tgz . && tar -zxvf content.tgz

  echo "Generating index ..."
  python ./search-indexer.py > search-index.txt

  echo "Coy index to search server ..."
  ssh www@jz "cd ~/search && ls -l search-index.txt* && mv search-index.txt search-index.txt.${appendix}"
  scp search-index.txt www@jz:~/search/. 

  echo "Restart search server ..."
  ssh www@jz "cd ~/search && sh restart.sh"

  date

