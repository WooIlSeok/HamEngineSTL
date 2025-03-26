module;

import std.core;
import Common.Memory;
import Common.Assert;

export module HamSTL.Vector;

export namespace ham
{
    template <typename T>
    using Vector = std::vector<T>;
}