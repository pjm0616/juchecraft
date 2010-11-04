

local function die(...)
	print(...)
	os.exit(1)
end

-- column represents angle
local function grp_calculate_framenum(row, col)
	return row*17 + col
end



----------- data starts

-- working dir: /tools/grp_conv
--local lg_prefix = '../../res/ui/sdl/imgs/'
--local lg_png_load_prefix = '../../res_raw/ui/sdl/imgs/'
local lg_mpq_dir = '../../../mini_sc_data/'

-- working dir: /res_raw/ui/sdl/imgs
local lg_prefix = '../../../../res/ui/sdl/imgs/'
local lg_png_load_prefix = './'
local lg_mpq_dir = '../../../../../mini_sc_data/'


init_mpq(lg_mpq_dir)



local palettes = {
	units = grppal.load('tileset\\Platform.wpe')
}

local grplist = {
	['game\\icons.grp'] = {
		palette = palettes.units, 
		rules = {
			{	n = {0, 0};
				file = 'game/icons/mineral.dat';
			}, 
			{	n = {1, 1};
				file = 'game/icons/gas_zerg.dat';
			}, 
			{	n = {2, 2};
				file = 'game/icons/gas_terran.dat';
			}, 
			{	n = {3, 3};
				file = 'game/icons/gas_protoss.dat';
			}, 
			{	n = {4, 4};
				file = 'game/icons/supplies_zerg.dat';
			}, 
			{	n = {5, 5};
				file = 'game/icons/supplies_terran.dat';
			}, 
			{	n = {6, 6};
				file = 'game/icons/supplies_protoss.dat';
			}, 
			{	n = {7, 7};
				file = 'game/icons/energy.dat';
			}, 
			-- 7, 8, 9: they are same; zerg/terran/protoss energy icon
			{	n = {10, 10};
				file = 'game/icons/nothing.dat';
			}, 
			--[[{	n = {11, 11};
				file = 'game/icons/colorbar.dat';
			}, ]]
		}, 
	}, 
	
	['unit\\neutral\\min01.grp'] = {
		palette = palettes.units, 
		rules = {
			{	n = {0, 3};
				file = 'objects/neutral/mineral01.dat';
			}, 
		}, 
	}, 
	['unit\\neutral\\min01sha.grp'] = {
		palette = palettes.units, 
		draw_shadow = true, 
		rules = {
			{	n = {0, 3};
				file = 'objects/neutral/mineral01_shadow.dat';
			}, 
		}, 
	}, 
	
	['unit\\terran\\control.grp'] = {
		palette = palettes.units, 
		rules = {
			{	n = {0, 0};
				file = 'objects/terran/buildings/command_center.dat';
			}, 
			{	n = {1, 1};
				file = 'objects/terran/buildings/command_center_building.dat';
			}, 
			--[[{	n = {2, 5};
				file = 'objects/terran/buildings/command_center_lifting.dat';
			}, ]]
		}, 
	}, 
	['unit\\terran\\marine.grp'] = {
		palette = palettes.units, 
		rules = {
			{	row = {0, 1};
				create_hflipped_imgs = true;
				file = 'objects/terran/units/marine_attack_start.dat';
			}, 
			{	row = {2, 3};
				create_hflipped_imgs = true;
				file = 'objects/terran/units/marine_attack.dat';
			}, 
			{	row = {4, 12};
				create_hflipped_imgs = true;
				file = 'objects/terran/units/marine_walk.dat';
			}, 
			{	n = {221, 228};
				file = 'objects/terran/units/marine_dying.dat';
			}, 
		}, 
	}, 
	
	['unit\\terran\\firebat.grp'] = {
		palette = palettes.units, 
		rules = {
			{	row = {0, 1};
				create_hflipped_imgs = true;
				file = 'objects/terran/units/firebat_attack.dat';
			}, 
			{	row = {2, 9};
				create_hflipped_imgs = true;
				file = 'objects/terran/units/firebat_walk.dat';
			}, 
		}, 
	}, 
	['unit\\zerg\\zergling.grp'] = {
		palette = palettes.units, 
		rules = {
			{	row = {0, 3};
				create_hflipped_imgs = true;
				file = 'objects/zerg/units/zergling_attack.dat';
			}, 
			{	row = {4, 11};
				create_hflipped_imgs = true;
				file = 'objects/zerg/units/zergling_walk.dat';
			}, 
			{	row = {4, 11};
				create_hflipped_imgs = true;
				file = 'objects/zerg/units/zergling_walk.dat';
			}, 
			{	row = {12, 15};
				create_hflipped_imgs = true;
				file = 'objects/zerg/units/zergling_burrow.dat';
			}, 
			{	n = {289, 295};
				file = 'objects/zerg/units/zergling_dying.dat';
			}, 
		}, 
	}, 
	['unit\\zerg\\zzeshad.grp'] = {
		palette = palettes.units, 
		draw_shadow = true, 
		rules = {
			{	row = {0, 3};
				create_hflipped_imgs = true;
				file = 'objects/zerg/units/zergling_attack_shadow.dat';
			}, 
			{	row = {4, 11};
				create_hflipped_imgs = true;
				file = 'objects/zerg/units/zergling_walk_shadow.dat';
			}, 
			{	row = {12, 15};
				create_hflipped_imgs = true;
				file = 'objects/zerg/units/zergling_burrow_shadow.dat';
			}, 
			{	n = {289, 295};
				file = 'objects/zerg/units/zergling_dying_shadow.dat';
			}, 
		}, 
	}, 
	-- add more images here
}


local pnglist = {
	['game/tconsole.png'] = {
		file = 'game/tconsole.dat';
	}, 
	['game/icons/juche_supplies_icon.png'] = {
		file = 'game/icons/supplies_juche.dat';
	}, 
	
	['objects/juche/units/52001.Juche_AojiWorker.png'] = {
		file = 'objects/juche/units/52001.Juche_AojiWorker.dat';
	}, 
	['objects/juche/buildings/53002.Juche_RodongCorrectionalFacility.png'] = {
		file = 'objects/juche/buildings/53002.Juche_RodongCorrectionalFacility.dat';
	}, 
	-- add more images here
}




for file, info in pairs(pnglist) do
	print('processing', file)
	local imgdata, imginfo = load_image(lg_png_load_prefix .. file)
	
	local img = jcimage.new({max_width = imginfo.width, max_height = imginfo.height})
	img:insertNewImage(imginfo, imgdata)
	img:save(lg_prefix .. info.file)
end



for file, info in pairs(grplist) do
	print('processing', file)
	local grp = grp.load(file)
	local grpinfo = grp:info()
	
	for _, rule in pairs(info.rules) do
		local img = jcimage.new(grpinfo)
		
		if rule.n then -- specific frame
			for n = rule.n[1], rule.n[2] do
				local imgdata, imginfo = grp:render(info.palette, n, false, false, 0x00000000, info.draw_shadow)
				img:insertNewImage(imginfo, imgdata)
			end
		
		elseif rule.row then -- units with angles
			for row = rule.row[1], rule.row[2] do
				-- not flipped
				for col = 0, 16, 1 do
					local n = grp_calculate_framenum(row, col)
					local imgdata, imginfo = grp:render(info.palette, n, false, false, 0x00000000, info.draw_shadow)
					img:insertNewImage(imginfo, imgdata)
				end
				if rule.create_hflipped_imgs then
					-- hoirzontally flipped
					for col = 16, 0, -1 do
						local n = grp_calculate_framenum(row, col)
						local imgdata, imginfo = grp:render(info.palette, n, true, false, 0x00000000, info.draw_shadow)
						img:insertNewImage(imginfo, imgdata)
					end
				end
			end
			
		else
			die('ERROR: invalid rule')
		end
		
		img:save(lg_prefix .. rule.file)
	end
end











