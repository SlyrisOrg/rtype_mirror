--
-- Created by IntelliJ IDEA.
-- User: milerius
-- Date: 23/01/18
-- Time: 11:02
-- To change this template use File | Settings | File Templates.
--

x = 1;
y = 2;
w = 1;
h = 2;
winWidth = 1920;
winHeight = 1080;

hp = 0;
attack = 0;
defense = 0;
speed = 0;
shield = 0;

hp_ori = 0;
attack_ori = 0;
defense_ori = 0;
speed_ori = 0;
shield_ori = 0;

forced_move = false;
nb_move = 40;

function internal_init(meID, hp_, attack_, defense_, speed_, shield_)
    hp = hp_;
    attack = attack_;
    defense = defense_;
    speed = speed_;
    shield = shield_;

    hp_ori = hp_;
    attack_ori = attack_;
    defense_ori = defense_;
    speed_ori = speed_;
    shield_ori = shield_;
end

function internal_moveRight(playerID, timeSinceLastFrame)
    local ettPlayer = getEntity(playerID);
    local box = ettPlayer:getBoundingBoxComponent();
    local anim = ettPlayer:getAnimationComponent();
    local playerPos = box:getPos();
    local relativePos = box:getRelativePos();
    local playerSize = box:getSize();
    local limitX = (winWidth - playerSize[w]);
    if playerPos[x] <= limitX
    then
        box:setPosition(playerPos[x] + (speed * timeSinceLastFrame), playerPos[y]);
        anim:setPosition(playerPos[x] - relativePos[x], playerPos[y] - relativePos[y]);
        quadMove(playerID);
    end
end

function internal_moveLeft(playerID, timeSinceLastFrame)
    local ettPlayer = getEntity(playerID);
    local box = ettPlayer:getBoundingBoxComponent();
    local anim = ettPlayer:getAnimationComponent();
    local playerPos = box:getPos();
    local relativePos = box:getRelativePos();
    if playerPos[x] >= 0
    then
        box:setPosition(playerPos[x] - (speed * timeSinceLastFrame), playerPos[y]);
        anim:setPosition(playerPos[x] - relativePos[x], playerPos[y] - relativePos[y]);
        quadMove(playerID);
    end
end

function internal_moveUp(playerID, timeSinceLastFrame)
    local ettPlayer = getEntity(playerID);
    local box = ettPlayer:getBoundingBoxComponent();
    local anim = ettPlayer:getAnimationComponent();
    local playerPos = box:getPos();
    local relativePos = box:getRelativePos();
    if playerPos[y] >= 0
    then
        box:setPosition(playerPos[x], playerPos[y] - (speed * timeSinceLastFrame));
        anim:setPosition(playerPos[x] - relativePos[x], playerPos[y] - relativePos[y]);
        quadMove(playerID);
    end
end

function internal_moveDown(playerID, timeSinceLastFrame)
    local ettPlayer = getEntity(playerID);
    local box = ettPlayer:getBoundingBoxComponent();
    local anim = ettPlayer:getAnimationComponent();
    local playerPos = box:getPos();
    local relativePos = box:getRelativePos();
    local playerSize = box:getSize();
    local limitY = (winHeight - playerSize[h]);
    if playerPos[y] <= limitY
    then
        box:setPosition(playerPos[x], playerPos[y] + (speed * timeSinceLastFrame));
        anim:setPosition(playerPos[x] - relativePos[x], playerPos[y] - relativePos[y]);
        quadMove(playerID);
    end
end

function internal_collide(meID, collideID)
    speed = -1500;
    forced_move = true;
    nb_move = 7;
    hp = hp - (0.07 * getEntity(collideID):getStatComponent().attack) - getEntity(meID):getStatComponent().defense;
    getEntity(meID):getStatComponent().hp = hp;
end

function internal_update(meID, timeSinceLastFrame)
    if forced_move then
        if nb_move == 0 then
            speed = speed_ori;
        else
            nb_move = nb_move - 1;
        end
    end
end

playerTable = {
    init=internal_init;
    onCollide = internal_collide;
    update = internal_update;
    moveUp = internal_moveUp;
    moveDown = internal_moveDown;
    moveLeft = internal_moveLeft;
    moveRight = internal_moveRight;
}