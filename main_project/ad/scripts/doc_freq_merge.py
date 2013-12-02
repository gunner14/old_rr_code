#!/usr/bin/env python
# encoding: utf-8

import sys
import re
sys.path.append('/home/muyuwei/collective-intelligence/')

import math
from operator import itemgetter
import mmseg
from doc_freq import GetDocFreqency

def DeviationRate(vec):
  total = 0.0
  for v in vec:
    total += v
  average = total / len(vec)
  variance = 0.0
  for v in vec:
    variance += (v - average) ** 2
  return (variance ** 0.5) / total

class DocCatProbClassifier:
  def __init__(self, cats):
    self.stop_words_ = set()
    self.cat_word_weight_ = {}
    self.cat_id_name_map_ = {-1 : 'unknown'}

    word_cat_prob = {}
    idx = 0
    cat_count = len(cats)
    for cat in cats:
      f = open(cat + '.df')
      for line in f:
        values = line.split()
        df = int(values[0])
        doc_count = int(values[2])
        word = values[3]
        word_cat_prob.setdefault(word, [0.0 for x in range(cat_count)])
        word_cat_prob[word][idx] = 100.0 * df / doc_count
      self.cat_id_name_map_[idx]  = cat
      idx += 1

    for i in range(cat_count):
      self.cat_word_weight_[i] = {}

    for word, dfv in word_cat_prob.items():
      deviation_rate = DeviationRate(dfv)
      max_df = 0.0
      for df in dfv:
        if df > max_df:
          max_df = df

      if max_df >= 0.3 and deviation_rate >= 0.68:
        for i in range(len(dfv)):
          #print dfv[i],
          if dfv[i] > 0.0:
            self.cat_word_weight_[i][word] = dfv[i]
        #print word, 'deviation', deviation_rate
      else:
        self.stop_words_.add(word)

  def Classify(self, text):
    text = re.sub(r'\d', r' ', text)
    text_words = [w for w in mmseg.seg_txt(text)]
    #print text_words
    category = -1
    max_weight = 0.0
    best_unknown_weight = 0.0

    for cat_id, word_weights in self.cat_word_weight_.items():
      #print '---------------------------'
      weight = 0.0
      unknown_weight = 0.0
      for word in text_words:
        if len(word.strip()) == 0:
          continue
        if word in word_weights:
          w = word_weights[word] * (3 ** ((len(word) - 1) / 3))
          weight += w
          unknown_weight -= len(word) * 0.87
          #print word, w
        else:
          if word not in self.stop_words_:
            if not word[0].isalpha(): # 忽略掉不认识的英文单词
              unknown_weight += len(word) * 1.0
            #print word, 'unknown'
          else:
            #print word, 'stop word'
            pass
      #print 'unknown_weight', unknown_weight
      if weight > max_weight and unknown_weight < 0.0:
        max_weight = weight
        category = cat_id
        best_unknown_weight = unknown_weight
 
    # print text, ':', category, self.cat_id_name_map_[category], max_weight, best_unknown_weight
    return self.cat_id_name_map_[category]

if __name__ == '__main__':
  cats = ('food', 'clothing', 'finance', 'history', 'sports')
  classifier = DocCatProbClassifier(cats)
  ads = ['伍度精品男装店', '皙肤泉祛痘挤痘', '俏佳人宝贝衣橱', '高雅气质女鞋,凉鞋打折季', 
      '卡尼尔美白',
      '三国演义',
      '岳飞是抗金英雄',
      '你想吃什么',
      '外汇黄金投资找我',
      '卸妆油49元特卖',
      '尚透社，时尚透了',
      '看三国玩三国',
      '买韩国服装',
      '新款女装上架！！',
      '经典款斜挎男包',
      '少看水浒，老看三国',
      '丰胸洁肤祛痘脱毛',
      '我想去逛街',
      '淘你所爱',
      '天堂伞9.99元起',
      '绿瘦减肥胶囊',
      '美白精华无效退款',
      '明星的不老秘密',
      '祛皱祛痘祛黑头',
      '卡扎菲邀俄中印投资石油业 西方慎对介入',
      '环球企业家]卫哲：黯然谢幕',
      '魔力挺内衣一穿就挺，越穿越挺，解决女性胸部6大问题：扁平、萎缩、下垂、附乳、外扩、含胸',
      '赫衣伦魔束衣 1拉，2铲，3锁定，塑造S形完美身段，减腰收腹、美背减脂、拢胸防垂、翘臀美腿',
      '"健美猪"真相："瘦肉精"猪肉进入双汇公司',
      '住建部将定期复审保障房家庭',
      '明天北京天气：晴转多云，气温高温 13℃/低温 0℃，无持续风向，微风',
      '这是什么类型',
      '玉兰油美白防晒套',
      '北京时间3月15日早间消息，苹果3月11日向欧洲商标局申请获得Apple Corps公司的青苹果Logo，两家公司长达20余年的商标权之争有望画上句号。',
      '当传统武侠遇上时尚，结果会是怎样？《天龙八部2》第四章“飞龙在天”3月18日即将开启豪气公测，全球8000万注册玩家，与7组新增服务器，共同迎接龙腾九天的武侠飞行时代，而新服专属的“飞龙在天探宝之旅”详细计划也于今日首次曝光，包含Louis Vuitton皮包与Iphone4的时尚大奖，将于公测之日起等待幸运儿的即时秒杀!',
      '梦幻系花园发型 打造复古70风潮',
      '6款初春百搭发型 适合所有脸型',
      '想出国，跟我走！',
      '2010秋冬装系列【女】。欧美。日。韩。',
      '香死人的土豆吃法：五香粉蒸土豆',
      '向葵花籽的蛋白质部分含有抑制睾丸成分，能引起睾丸萎缩，影响正常的生育功能，故育龄青年不宜多食。',
      '这本质跟菜场卖菜的阿姨做的事情是一样的。 　　 　　一件简单的事情，为什么非要搞得那么复杂，你非要过着苦行僧的生活？ 　　 　　未来的行情谁也无法预测，你唯一用到的东西就是规则，一致性的交易规则，它让你站在这场概率游戏的大数一侧。 　　 　　什么思想，什么理念，什么哲学，那些是分析师用过糊弄交易者的语录罢了。 　　 　　真正的交易者，只关心两件事：1、我开仓后行情证明我对了怎么办；2、我开仓后行情证明我错了怎么办； 　　 　　盈利不是靠你预测行情的胜率来获取的，而是依赖“你做错的时候你尽可能少亏，你做对的时候你尽可能多赚”，这就是trader和分析师的最大差别。 艾mm开仓的目的就是要拿到直到尾盘的大波段么？',
      '阿比达尔将在本周五进行肝脏肿瘤的摘除手术，他的突然患病令足坛群星们相当震惊。巴萨球员们纷纷通过微博向阿比达尔送去了祝福，皇马球员卡卡也号召球员们一起为阿比达尔祈祷。在生命面前，竞争和敌对变得微不足道。也让我们一起祝愿阿比达尔早日康复~',
      '女人养颜养生：6款美白祛斑食谱，让你更美',
      '吃三文鱼、沙丁鱼等鱼类。',
      '韩版正品牛仔布包三STREET韩版正品牛仔布包单扣皮带装饰',
      '品质如影随形黄金比例至尊天然牛皮手袋源自意大利名门',
      '投资实践：对时间的不同理解 原文地址： 投资实践：对时间的不同理解 作者： 一只花蛤 青年时代曾读过许多作家的作品，大名鼎鼎的路易斯·博尔赫斯就是其中一位。博尔赫斯常常以哲学想象构思故事，沉溺于微光中的精神冥想。时间是他笔下的重头戏，他在他的很多作品中讨论了时间问题，《秘密的奇迹》就是其中神奇的一篇。它叙述的是一个捷克作家因为被德国法西斯查出犹太血统而被捕。经过审讯，决定于3月29日上午9时执行枪决。但是因为他还有一部剧作没有完成，于是他祈求上帝给他一年时间，上帝答应了他。当9点钟正行刑队的枪口对准他的时候，奇迹出现了',
      '羽西Yuesai (1992年华裔名人靳羽西女士创建,最受欢迎的化妆品品牌,现属于欧莱雅集团)',
      '高丝雪肌精 (于1985年推出,日本高丝化妆品公司的美白系列,高丝化妆品有限公司)',
      '兰芝Laneige  (于1994年韩国,令韩国年轻时尚女性着迷的化妆品品牌,爱茉莉太平洋集团)',
      '奇才队的困局',
      '中国体操又陷“裁判门” 邵斌近期或被禁止执法',
      '一五旬辽宁籍男子伙同他人从天津抵忻后，刚下火车便窜至忻府区砸碎汽车玻璃实施盗窃。近日，该男子雷某被忻府区公安局作出行政拘留15日的处罚。3月12日12时许，忻府区公安局纪检书记刘国兴途经七一北路一超市附近时，发现两名中年男子神色']

  #ads = [
  #    '投资实践：对时间的不同理解 原文地址： 投资实践：对时间的不同理解 作者： 一只花蛤 青年时代曾读过许多作家的作品，大名鼎鼎的路易斯·博尔赫斯就是其中一位。博尔赫斯常常以哲学想象构思故事，沉溺于微光中的精神冥想。时间是他笔下的重头戏，他在他的很多作品中讨论了时间问题，《秘密的奇迹》就是其中神奇的一篇。它叙述的是一个捷克作家因为被德国法西斯查出犹太血统而被捕。经过审讯，决定于3月29日上午9时执行枪决。但是因为他还有一部剧作没有完成，于是他祈求上帝给他一年时间，上帝答应了他。当9点钟正行刑队的枪口对准他的时候，奇迹出现了'
  #    ]
  for ad in ads:
    print classifier.Classify(ad), ad

  #f = open('t.txt', 'r')
  #for line in f:
  #  print classifier.Classify(line), line

