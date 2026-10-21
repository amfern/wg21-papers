# FAQ:

Q: We shouldn't be using enums class in the first place, and we should have something better, or improve bitset..
   In a sense we should decouple the bitset "tool" from the underlying bits representation
   Conflation of ideas. enum bitflags are easy, but not simple. They complect naming bits (what we actually want) with representing them. True, it’s not our fault; this is the legacy of C that multiple languages double down on at this point. But it’s also not right. We say “the type has members A and B” then we pretend that A|B is also a member of the type. But that’s immediately subverting type safety: we have confused representation with naming.
   https://www.reddit.com/r/cpp/comments/1v0pizv/comment/oyhlddi/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
A: This is true, it can be a wrapper type, or a bitfield appraoch as as propsoed here https://godbolt.org/z/Yvn7r38z7, but that would be a new type in C++ language, and therefore a different paper
   Or maybe it better to extend bitset to have type safety bitwise operators

Q: The error handling for debug things in the ranges library can be confused with pipes, and show unrelated errors.
A: It shouldn't be a big problem

Q: Will it increase compilation time
A: trivial increase, and it's in own flag


Q: it’s still going to be kind of annoying to apply bit operations to enumeration types you don’t own.
A: it is possible (see enable_bitmask_type), but we want to keep it out of this proposal for simplicity sake


Q: Aesthetics. I don’t want using these things to look like using plain old integers, because using plain old integers is fraught with peril. Which leads to…
A: it probably make sense to add .set(), .get(), .union(), .inverse() functions to class enum, however since class enum not allowed to have function, it's not possible.
   We can make assigment via subscript operator like so  "a2[A::x] = true;", but probably in a different proposal



## public links:

A better bitset for enum flags:
https://www.elbeno.com/blog/?p=1836
https://www.reddit.com/r/cpp/comments/1v0pizv/comment/oyj5wvw/

The complement of true is true, except when it's false:
https://dryperspective.github.io/posts/complement-of-true/
https://www.reddit.com/r/cpp/s/wpkRU9tmOq


## emails:
Hello, I'm a new member of WG21, and after reading through the latest revision of P4313 in the 2026-08 mailing list, I had some questions regarding the equivalence operator.

The idea is explained quite well in [this article](https://dalzhim.github.io/2017/08/11/Improving-the-enum-class-bitmask/), but it boils down to the common mistake of using == when comparing a bitmask to a set of flags, when what's actually required is bitwise &, and then comparison.

In my view, ideally, comparing a mask to a flag would be a compile-time error. Would it be feasible to include this in your approach? Or possibly, is it not actually desirable?

Thank you in advance,
-- 
Éidhne Kennedy


Hello Eidhne,

I understand the mistake this solution is trying to prevent, but I think it would also prevent valid use cases.
Consider the following example:

enum class Permission {
  None    = 0,
  Read    = 1 << 0,
  Write   = 1 << 1,
  Execute = 1 << 2,
};
enableEnumClassBitmask(Permission);

auto someMask() {
  return Permission::Read | Permission::Write;
};

int main(int argc, const char * argv[])
{
  auto perm = someMask();

  // 1.
  // Now i am not able to check if only one flag is set
  // if (perm == Permission::Read) { 
  // }
  // I can fix this by converting the right-hand side to a bitmask type.
  // However, I'd argue that converting enumarator to bitmask where appropriate is no different
  // from simply making sure to use the correct operator == or &, when appropriate.
  if (perm == make_bitmask(Permission::Read)) { 
 
  }

  // Also I can fix the following way, which leads me to point 2.
  if ((perm & Permission::Read) == Permission::Read) {

  }

  // 2.
  // I think the issue the article is trying to solve is largely self-inflicted
  // by allowing a `bitmask` to implicitly evaluate to bool when needed.
  //
  // For example, the following evaluates to true even though Permission::Execute
  // is not set in `perm`.
  auto perm2 = Permission::Read | Permission::Execute;
  if (perm & perm2) {
    // i shouldn't be here
  }

  // Since the correct thing is to compare the AND product to the enum value anyway,
  // requiring an explicit comparison also mitigates the issue the article was trying to solve.
  if ((perm & perm2) == perm2) {

  }
}


3.
// The solution necessitates a type distinction which leads to awkwardness in the code.
// A change to the bitmask value changes the type inferred by `auto`,
// which can then change which operators are available and lead to
// cascading changes in the code from `&` to `==`.
auto someMask() {
-  return Permission::Read | Permission::Write;
+ return Permission::Read;
};

int main(int argc, const char * argv[])
{
  auto perm = someMask();

-  if (perm == make_bitmask(Permission::Read)) {
+ if (perm == Permission::Read) {

  }
}


I think comparing an enum value with a bitmask is a valid use case and shouldn't necessarily result in a compile-time error, 
and this is to mind the appropriate usage of as a usage of `&` and `==` rather than a compile-time restriction.
And apologies for the late reply.
