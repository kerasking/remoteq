# create by qianqians
# 2014-10-22

# used it by include remote_queue.h
# and link remote-queue.lib

# maybe remote_queue.h could import the header file conflict
# so include remote_queue.h before any other header file
# and once a header file include remote_queue.h include it before any other header file

# only edge mode
# so pop used like this
# while (remote_queue::pop(ch, cmd, bugtocmd))

# example in remote_queue_server remote_queue_client