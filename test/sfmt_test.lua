local tostring = tostring
local testcase = require('testcase')
local sfmt = require('sfmt')

local TSET_SEED = os.time()

function testcase.before_each()
    sfmt.init(TSET_SEED)
end

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

function testcase.random()
    -- test that rand* funcs
    for fn, exp in pairs({
        -- test that rand32 func generate 32bit random number
        rand32 = '^%d+$',
        -- test that rand64 func generate 64bit random number
        rand64 = '^%-?[%de+.]+$',
    }) do
        -- test that generate <N>bit random number
        sfmt.init(TSET_SEED)
        local v = sfmt[fn]()
        assert.match(tostring(v), exp, false)

        -- test that fn method returns the same value as sfmt[fn] func
        local s = assert(sfmt.new(TSET_SEED))
        assert.equal(s[fn](s), v)
    end

    -- test that real* and res* funcs
    for _, fn in ipairs({
        'real1',
        'real2',
        'real3',
        'res53',
        'res53mix',
    }) do
        sfmt.init(TSET_SEED)
        -- test that real<N> func generate random number on [0,1]
        local v = sfmt[fn]()
        assert.less(v, 1)

        -- test that fn method returns the same value as sfmt[fn] func
        local s = assert(sfmt.new(TSET_SEED))
        assert.equal(s[fn](s), v)
    end
end

