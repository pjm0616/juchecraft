

local function die(...)
	print(...)
	os.exit(1)
end

-- column represents angle
local function grp_calculate_framenum(row, col)
	return row*17 + col
end



----------- data starts

local lg_prefix = '/home/pjm0616/Desktop/tmp/res/'

local palettes = {
	units = grppal.load('tileset\\Platform.wpe')
}

local grplist1 = {
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
				file = 'unit/neutral/mineral01.dat';
			}, 
		}, 
	}, 
	['unit\\neutral\\min01sha.grp'] = {
		palette = palettes.units, 
		rules = {
			{	n = {0, 3};
				file = 'unit/neutral/mineral_shadow01.dat';
			}, 
		}, 
	}, 
	
	['unit\\terran\\control.grp'] = {
		palette = palettes.units, 
		rules = {
			{	n = {0, 0};
				file = 'unit/terran/command_center.dat';
			}, 
			{	n = {1, 1};
				file = 'unit/terran/command_center_building.dat';
			}, 
			--[[{	n = {2, 5};
				file = 'unit/terran/command_center_lifting.dat';
			}, ]]
		}, 
	}, 
	['unit\\terran\\marine.grp'] = {
		palette = palettes.units, 
		rules = {
			{	row = {0, 1};
				create_hflipped_imgs = true;
				file = 'unit/terran/marine_attack_start.dat';
			}, 
			{	row = {2, 3};
				create_hflipped_imgs = true;
				file = 'unit/terran/marine_attack.dat';
			}, 
			{	row = {4, 12};
				create_hflipped_imgs = true;
				file = 'unit/terran/marine_walk.dat';
			}, 
			{	n = {221, 228};
				file = 'unit/terran/marine_dying.dat';
			}, 
		}, 
	}, 
	
	['unit\\terran\\firebat.grp'] = {
		palette = palettes.units, 
		rules = {
			{	row = {0, 1};
				create_hflipped_imgs = true;
				file = 'unit/terran/firebat_attack.dat';
			}, 
			{	row = {2, 9};
				create_hflipped_imgs = true;
				file = 'unit/terran/firebat_walk.dat';
			}, 
		}, 
	}, 
	['unit\\zerg\\zergling.grp'] = {
		palette = palettes.units, 
		rules = {
			{	row = {0, 3};
				create_hflipped_imgs = true;
				file = 'unit/zerg/zergling_attack.dat';
			}, 
			{	row = {4, 11};
				create_hflipped_imgs = true;
				file = 'unit/zerg/zergling_walk.dat';
			}, 
			{	row = {4, 11};
				create_hflipped_imgs = true;
				file = 'unit/zerg/zergling_walk.dat';
			}, 
			{	row = {12, 15};
				create_hflipped_imgs = true;
				file = 'unit/zerg/zergling_burrow.dat';
			}, 
			{	n = {289, 295};
				file = 'unit/zerg/zergling_dying.dat';
			}, 
		}, 
	}, 
	['unit\\zerg\\zzeshad.grp'] = {
		palette = palettes.units, 
		draw_shadow = true, 
		rules = {
			{	row = {0, 3};
				create_hflipped_imgs = true;
				file = 'unit/zerg/zergling_shadow_attack.dat';
			}, 
			{	row = {4, 11};
				create_hflipped_imgs = true;
				file = 'unit/zerg/zergling_shadow_walk.dat';
			}, 
			{	row = {4, 11};
				create_hflipped_imgs = true;
				file = 'unit/zerg/zzeshad_walk.dat';
			}, 
			{	row = {12, 15};
				create_hflipped_imgs = true;
				file = 'unit/zerg/zergling_shadow_burrow.dat';
			}, 
			{	n = {289, 295};
				file = 'unit/zerg/zergling_shadow_dying.dat';
			}, 
		}, 
	}, 
	-- add more images here
}




for file, info in pairs(grplist1) do
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





