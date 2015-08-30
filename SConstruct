import os
def list_dir(dir):
	all_dirs = []
	for root, dirs, files in os.walk('./', True):
		for name in dirs:
			cur_dir = os.path.join(root, name)
			if cur_dir.find(".git") < 0:
				all_dirs.append(cur_dir)
	return all_dirs

cc_flags = {'CCFLAGS' : ['-ggdb', '-rdynamic']}
inc_flags = {'CPPPATH':['async-redis-cli/example', 'async-redis-cli/libevent-session'
, 'async-redis-cli/redis-cli',  'async-redis-cli/unix-include'
, 'async-redis-cli/unix-include/event2']}
obj = Glob('*.cpp')
all_dirs = list_dir('./')
print all_dirs
for dir in all_dirs:
	obj += Glob(dir + '/*.cpp')

lib1 = File('async-redis-cli/unix-include/lib/libevent.a')
lib2 = File('async-redis-cli/unix-include/lib/libevent_core.a')
lib3 = File('async-redis-cli/unix-include/lib/libevent_extra.a')
env = Environment(CXXFLAGS="-std=c++0x") 
env.MergeFlags(inc_flags)
env.Program('xj_redis_cli', list(obj), LIBS=[lib1, lib2, lib3, 'pthread'])
