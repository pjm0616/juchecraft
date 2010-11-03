

pal = grppal.load('tileset\\Platform.wpe')
grp = grp.load('unit\\terran\\control.grp')
grpinfo = grp:info()

data, info = grp:render(pal, 0, false, false, 0x00000000, false)
data2, info2 = grp:render(pal, 1, false, false, 0x00000000, false)
for k,v in pairs(info) do
	print(k,v)
end
print('pixel data: ', #data, 'bytes')


img = jcimage.new(grpinfo)
img:insertNewImage(info, data)
img:insertNewImage(info2, data2)
img:save('./test/test.dat')


