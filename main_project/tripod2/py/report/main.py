import os.path

import tornado.httpserver
import tornado.ioloop
import tornado.options
import tornado.web

import db_api
import image_api

from tornado.options import define, options
define('port', default=80, help='tripod report server runs on the given port', type=int)

class IndexHandler(tornado.web.RequestHandler):
    def get(self):
        bizes = db_api.conndb.GetBizes()
        caches = db_api.conndb.GetCaches()
        self.render('index.tpl',biz_items = bizes, machine_items= caches)
        
class BizReportHandler(tornado.web.RequestHandler):
    def post(self):
#        try:
            begin = self.get_argument('begin_time_biz')
            end = self.get_argument('end_time_biz') 
            biz = self.get_argument('biz_id')
            data =  db_api.conndb.ReadData(biz,['get','set','load','remove'], begin ,end)
            image_src = image_api.PlotImage(biz,['get','set','load','remove'], begin,end,data)
            bizes = db_api.conndb.GetBizes()
            self.render('biz_query.tpl', selected_biz_id =biz, biz_items = bizes, begin_time = begin, end_time= end, items = data,pic_path= image_src)
#        except Exception:
#            self.render('none.tpl')

class CacheReportHandler(tornado.web.RequestHandler):
    def post(self):
        machine = self.get_argument('machine')
        begin = self.get_argument('begin_time_cache')
        end = self.get_argument('end_time_cache') 
        data = db_api.conndb.ReadData(machine,['used_memory_rss','total','hit','miss'], begin ,end)
        image_src_memory = image_api.PlotImage(machine ,['used_memory_rss'], begin,end,data)
        image_src_command = image_api.PlotImage(machine ,['total','hit','miss'], begin,end,data)
        caches = db_api.conndb.GetCaches()
        self.render('cache_query.tpl', selected_cc =machine, machine_items= caches, begin_time = begin, end_time= end, items = data, pic_path_memory= image_src_memory, pic_path_command= image_src_command)
        
if __name__ == '__main__':
    tornado.options.parse_command_line()
    app = tornado.web.Application(
        handlers=[(r'/', IndexHandler), 
                  (r'/cache_query', CacheReportHandler),
                  (r'/biz_query', BizReportHandler)],
        template_path=os.path.join(os.path.dirname(__file__), './templates'),
        static_path=os.path.join(os.path.dirname(__file__), './static'),
    )
    http_server = tornado.httpserver.HTTPServer(app)
    http_server.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()

