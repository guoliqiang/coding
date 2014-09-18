#!/usr/bin/env python
#encoding=utf-8
#
# Copyright 2014 Liqiang Guo. All Rights Reserved.
# Author: Liqiang Guo (guol@garena.com)kk
# Date  : 2014-09-16 04:11:28
# File  : Buzz.py
# Brief :

import sys
sys.path.append("../base/")
import base
base.InitSysPath()

import client
import test_pb2
import logging
import logging.handlers
import gevent
import time
import threading
import base
import random
import iclient

logger = logging.getLogger(__name__)

# offline
ip = '203.116.50.129'
port = 30007

# online
#ip = '203.116.50.139'
#port = 30006

def BuzzGeoReport(client, lat, log, id):
  request = test_pb2.BuzzGeoReport()
  request.requestId = "99"
  request.info.latitude = lat
  request.info.longitude = log
  request.info.buzz_id = id
  logging.info("GeoReportRequest:%s",request)
  print request
  client.send_message(20, request)

def BuzzGeoReportResponse(client, msg):
  response = test_pb2.BuzzGeoReportResponse.FromString(msg)
  logging.info("GeoReportResponse:%s", response)

def BuzzGeo(client, lat, lon, id):
  request = test_pb2.BuzzGeoReport()
  request.requestId = "99"
  request.info.latitude = lat
  request.info.longitude = lon
  request.info.buzz_id = id
  msg = client.send_message(20, request)
  response = test_pb2.BuzzGeoReportResponse.FromString(msg)
  print response

def BuzzGeoFindNeighboursRequest(client, lat, lon, cursor, offset):
  request = test_pb2.BuzzGeoFindNeighboursRequest()
  request.requestId = "99"
  request.latitude = lat
  request.longitude = lon
  request.cursor_pos = cursor
  request.offset = offset
  logging.info("GeoFindNeighboursRequest:%s", request)
  client.send_message(21, request)

def BuzzGeoFindNeighboursRequestResponse(client, msg):
  response = test_pb2.BuzzGeoFindNeighboursRequestResponse.FromString(msg)
  logging.info("GeoFindNeighboursRequestResponse:%s", response)

def BuzzGeoFindNeighbours(client, lat, lon, cursor, offset):
  request = test_pb2.BuzzGeoFindNeighboursRequest()
  request.requestId = "99"
  request.latitude = lat
  request.longitude = lon
  request.cursor_pos = cursor
  request.offset = offset
  msg = client.send_message(21, request)
  response = test_pb2.BuzzGeoFindNeighboursRequestResponse.FromString(msg)
  print response

class Thread(threading.Thread):
  def __init__(self, client):
    super(Thread, self).__init__()
    self.client = client

  def run(self):
    for j in range(0, 1):
      for i in range(0, 1):
        BuzzGeoReport(self.client, j + 50.005, j + 50.009,
            random.randint(0, 100000000))
        BuzzGeoReport(self.client, j + 50, j + 50,
            random.randint(0, 100000000))
        BuzzGeoReport(self.client, j + 50.000003, j + 50.000004,
            random.randint(0, 100000000))
        BuzzGeoReport(self.client, j + 50.009, j + 50.005,
            random.randint(0, 100000000))
      BuzzGeoFindNeighboursRequest(self.client, j + 50, j + 50, 0, 10)
      BuzzGeoFindNeighboursRequest(self.client, j + 50, j + 50, 2, 8)
      BuzzGeoFindNeighboursRequest(self.client, j + 50, j + 50, 3, 10)
      BuzzGeoFindNeighboursRequest(self.client, j + 50, j + 50, 4, 10)

      BuzzGeoFindNeighboursRequest(self.client, j + 30, j + 50, 4, 10)
      pass
    pass
  pass

def Asynchronous():
  client = client.Client(ip, port)
  client.add_handler(BuzzGeoReportResponse, 20)
  client.add_handler(BuzzGeoFindNeighboursRequestResponse, 21)
  try:
    send_thread = Thread(client)
    send_thread.start()
    client.start()
    client.join()
  except Exception, e:
    print e
    client.stop()

if __name__ == '__main__':
  base.InitLog()
  iclient = iclient.iClient(ip, port)
  iclient.connect();
  BuzzGeo(iclient, 50, 50, 50)
  BuzzGeoFindNeighbours(iclient, 50, 50, 0, 10)
