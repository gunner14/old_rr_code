from sqlalchemy import Column
from sqlalchemy.types import Integer, String

from feedadmin.model.meta import Base
from sqlalchemy.sql.expression import text

class TplWhitelist(Base):
    __tablename__ = "tpl_whitelist"

    id = Column(Integer, primary_key=True)
    tpl_id = Column(Integer)
    uid = Column(Integer)
    stype = Column(Integer)
    version = Column(Integer)

    def __init__(self, tpl_id, uid, stype, version, id):
         self.tpl_id = tpl_id
         self.uid = uid
         self.stype = stype
         self.version = version
         self.id = id

    def __repr__(self):
        return "<TplWhitelist %id>" % (self.id)
