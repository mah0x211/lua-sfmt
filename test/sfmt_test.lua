local tostring = tostring
local testcase = require('testcase')
local assert = require('assert')
local sfmt = require('sfmt')

local TEST_SEED = os.time()

function testcase.idstring()
    -- test that return version
    assert.is_string(sfmt.idstring())
end

function testcase.new()
    -- test that create smft
    local s = assert(sfmt.new())
    assert.match(tostring(s), 'sfmt: ')
end

function testcase.init()
    -- test that init func return nil
    assert.is_nil(sfmt.init(12, 34))

    -- test that init method smft and return self
    local s = assert(sfmt.new())
    local v = s:init(12, 34)
    assert.rawequal(v, s)
end

function testcase.rand32()
    -- test that generate 32bit random number
    for _ = 1, 100 do
        local v = sfmt.rand32()
        assert(0 <= v and v <= 0xFFFFFFFF, v)
    end

    -- test that method returns the same value
    sfmt.init(TEST_SEED)
    local v = sfmt.rand32()
    local s = assert(sfmt.new(TEST_SEED))
    assert.equal(s:rand32(), v)

    -- test that generate random number on [5,50]
    for _ = 1, 100 do
        v = sfmt.rand32(20)
        assert(20 >= v and v >= 0)

        v = sfmt.rand32(20, 5)
        assert(20 >= v and v >= 5)

        v = s:rand32(20, 5)
        assert(20 >= v and v >= 5)
    end
end

function testcase.rand64()
    -- test that generate 64bit random number
    for _ = 1, 100 do
        local v = sfmt.rand64()
        assert(0 <= v and v <= 0x7FFFFFFFFFFFFFFF, v)
    end

    -- test that method returns the same value
    sfmt.init(TEST_SEED)
    local v = sfmt.rand64()
    local s = assert(sfmt.new(TEST_SEED))
    assert.equal(s:rand64(), v)

    -- test that generate random number on [5,50]
    for _ = 1, 100 do
        v = sfmt.rand64(20)
        assert(20 >= v and v >= 0)

        v = sfmt.rand64(20, 5)
        assert(20 >= v and v >= 5)

        v = s:rand64(20, 5)
        assert(20 >= v and v >= 5)
    end
end

function testcase.real1()
    -- test that generate random number on [0,1] (0 <= v <= 1)
    for _ = 1, 100 do
        local v = sfmt.real1()
        assert(0 <= v and v <= 1)
    end

    -- test that method returns the same value
    sfmt.init(TEST_SEED)
    local v = sfmt.real1()
    local s = assert(sfmt.new(TEST_SEED))
    assert.equal(s:real1(), v)

    -- test that generate random number on [5,50]
    for _ = 1, 100 do
        v = sfmt.real1(20)
        assert(20 >= v and v >= 0)

        v = sfmt.real1(20, 5)
        assert(20 >= v and v >= 5)

        v = s:real1(20, 5)
        assert(20 >= v and v >= 5)
    end
end

function testcase.real2()
    -- test that generate random number on [0,1) (0 <= v < 1)
    for _ = 1, 100 do
        local v = sfmt.real2()
        assert(0 <= v and v < 1)
    end

    -- test that method returns the same value
    sfmt.init(TEST_SEED)
    local v = sfmt.real2()
    local s = assert(sfmt.new(TEST_SEED))
    assert.equal(s:real2(), v)

    -- test that generate random number on [5,50]
    for _ = 1, 100 do
        v = sfmt.real2(20)
        assert(20 >= v and v >= 0)

        v = sfmt.real2(20, 5)
        assert(20 >= v and v >= 5)

        v = s:real2(20, 5)
        assert(20 >= v and v >= 5)
    end
end

function testcase.real3()
    -- test that generate random number on (0,1) (0 < v < 1)
    for _ = 1, 100 do
        local v = sfmt.real3()
        assert(0 < v and v < 1)
    end

    -- test that method returns the same value
    sfmt.init(TEST_SEED)
    local v = sfmt.real3()
    local s = assert(sfmt.new(TEST_SEED))
    assert.equal(s:real3(), v)

    -- test that generate random number on [5,50]
    for _ = 1, 100 do
        v = sfmt.real3(20)
        assert(20 >= v and v >= 0)

        v = sfmt.real3(20, 5)
        assert(20 >= v and v >= 5)

        v = s:real3(20, 5)
        assert(20 >= v and v >= 5)
    end
end

function testcase.res53()
    -- test that generate random number on [0,1) (0 <= v < 1) with 53-bit resolution
    for _ = 1, 100 do
        local v = sfmt.res53()
        assert(0 <= v and v < 1)
    end

    -- test that method returns the same value
    sfmt.init(TEST_SEED)
    local v = sfmt.res53()
    local s = assert(sfmt.new(TEST_SEED))
    assert.equal(s:res53(), v)

    -- test that generate random number on [5,50]
    for _ = 1, 100 do
        v = sfmt.res53(20)
        assert(20 >= v and v >= 0)

        v = sfmt.res53(20, 5)
        assert(20 >= v and v >= 5)

        v = s:res53(20, 5)
        assert(20 >= v and v >= 5)
    end
end

function testcase.res53mix()
    -- test that generate random number on [0,1) (0 <= v < 1) with 53-bit resolution using two 32-bit integers
    for _ = 1, 100 do
        local v = sfmt.res53mix()
        assert(0 <= v and v < 1)
    end

    -- test that method returns the same value
    sfmt.init(TEST_SEED)
    local v = sfmt.res53mix()
    local s = assert(sfmt.new(TEST_SEED))
    assert.equal(s:res53mix(), v)

    -- test that generate random number on [5,50]
    for _ = 1, 100 do
        v = sfmt.res53mix(20)
        assert(20 >= v and v >= 0)

        v = sfmt.res53mix(20, 5)
        assert(20 >= v and v >= 5)

        v = s:res53mix(20, 5)
        assert(20 >= v and v >= 5)
    end
end

