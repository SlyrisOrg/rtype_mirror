--
-- Created by IntelliJ IDEA.
-- User: king
-- Date: 1/26/18
-- Time: 7:58 PM
-- To change this template use File | Settings | File Templates.
--

posX = 0;
posY = 0;

targetX_table = {};
targetY_table = {};

fireRate_table = {};

winWidth = 1920;
winHeight = 1080;

function internal_init(meID, targetX_, targetY_, fireRate_)
    targetX_table[meID] = targetX_;
    targetY_table[meID] = targetY_;
    fireRate_table[meID] = fireRate_;
end

function internal_collide(meID, collideID)
    print("halooo");
    local ettMe = getEntity(meID);
    local ettCol = getEntity(collideID);

    if ettCol:hasAlliedComponent()
    then
    end
    print("hollllaaaaa");
end

function internal_update(meID, timeSinceLastUpdate)
    local ettMe = getEntity(meID);
    local box = ettMe:getBoundingBoxComponent();
    local stat = ettMe:getStatComponent();
    local sprite = ettMe:getSpriteComponent();
    local Pos = box:getPos();
    local relativePos = box:getRelativePos();

    local xsign = 1;
    if Pos[x] > targetX_table[meID] then
        xsign  = -1;
    end
    local xtravel = Pos[x] + xsign * (stat.speed * timeSinceLastUpdate);
    local ysign = 1;
    if Pos[y] > targetY_table[meID] then
        ysign  = -1;
    end
    local ytravel = Pos[y] + ysign * (stat.speed * timeSinceLastUpdate);

    if Pos[x] ~= targetX_table[meID] then
        if xsign == -1 then
            if xtravel <= targetX_table[meID] then
                box:setPosition(targetX_table[meID], Pos[y]);
            else
                box:setPosition(xtravel, Pos[y]);
            end
        else
            if xtravel >= targetX_table[meID] then
                box:setPosition(targetX_table[meID], Pos[y]);
            else
                box:setPosition(xtravel, Pos[y]);
            end
        end
        sprite:setPosition(Pos[x] - relativePos[x], Pos[y] - relativePos[y]);
        quadMove(meID);
    end
    if Pos[y] ~= targetY_table[meID] then
        if ysign == -1 then
            if ytravel <= targetY_table[meID] then
                box:setPosition(Pos[x], targetY_table[meID]);
            else
                box:setPosition(Pos[x], ytravel);
            end
        else
            if ytravel >= targetY_table[meID] then
                box:setPosition(Pos[x], targetY_table[meID]);
            else
                box:setPosition(Pos[x], ytravel);
            end
        end
        sprite:setPosition(Pos[x] - relativePos[x], Pos[y] - relativePos[y]);
        quadMove(meID);
    end

    if stat.hp <= 0 then
        mark(meID);
    end
end

stayAndShootTable = {
    onCollide = internal_collide;
    init = internal_init;
    update = internal_update;
}