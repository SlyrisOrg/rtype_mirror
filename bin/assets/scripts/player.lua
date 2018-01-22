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

function moveRight(playerID, timeSinceLastFrame)
    local ettPlayer = getEntity(playerID);
    local box = ettPlayer:getBoundingBoxComponent();
    local anim = ettPlayer:getAnimationComponent();
    local playerPos = box:getPos();
    local relativePos = box:getRelativePos();
    local playerSize = box:getSize();
    local limitX = (winWidth - playerSize[w]);
    if playerPos[x] <= limitX
    then
        box:setPosition(playerPos[x] + (700 * timeSinceLastFrame), playerPos[y]);
        anim:setPosition(playerPos[x] - relativePos[x], playerPos[y] - relativePos[y]);
        quadMove(playerID);
    end
end

function moveLeft(playerID, timeSinceLastFrame)
    local ettPlayer = getEntity(playerID);
    local box = ettPlayer:getBoundingBoxComponent();
    local anim = ettPlayer:getAnimationComponent();
    local playerPos = box:getPos();
    local relativePos = box:getRelativePos();
    if playerPos[x] >= 0
    then
        box:setPosition(playerPos[x] - (700 * timeSinceLastFrame), playerPos[y]);
        anim:setPosition(playerPos[x] - relativePos[x], playerPos[y] - relativePos[y]);
        quadMove(playerID);
    end
end

function moveUp(playerID, timeSinceLastFrame)
    local ettPlayer = getEntity(playerID);
    local box = ettPlayer:getBoundingBoxComponent();
    local anim = ettPlayer:getAnimationComponent();
    local playerPos = box:getPos();
    local relativePos = box:getRelativePos();
    if playerPos[y] >= 0
    then
        box:setPosition(playerPos[x], playerPos[y] - (700 * timeSinceLastFrame));
        anim:setPosition(playerPos[x] - relativePos[x], playerPos[y] - relativePos[y]);
        quadMove(playerID);
    end
end

function moveDown(playerID, timeSinceLastFrame)
    local ettPlayer = getEntity(playerID);
    local box = ettPlayer:getBoundingBoxComponent();
    local anim = ettPlayer:getAnimationComponent();
    local playerPos = box:getPos();
    local relativePos = box:getRelativePos();
    local playerSize = box:getSize();
    local limitY = (winHeight - playerSize[h]);
    if playerPos[y] <= limitY
    then
        box:setPosition(playerPos[x], playerPos[y] + (700 * timeSinceLastFrame));
        anim:setPosition(playerPos[x] - relativePos[x], playerPos[y] - relativePos[y]);
        quadMove(playerID);
    end
end