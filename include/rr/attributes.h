#pragma once

#define CONCAT(x, y) x y

// options are: WithBase, WithPrivate
#define RR_REFLECT(options) [[clang::annotate(CONCAT("re:reflect:", options))]]

#define RR_ALIAS(name) [[clang::annotate(CONCAT("re:alias:", name))]]

#define RR_EXCLUDE