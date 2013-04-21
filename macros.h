#define MIN(a,b) ( ((a < b) ? a : b) )

// directivas del compilador para optimizar saltos
#define LIKELY(expr)   (__builtin_expect (expr, 1))
#define UNLIKELY(expr) (__builtin_expect (expr, 0))


#define INCREASE_IFNEEDED(buffer, count, toAdd, limit) do {\
if ( UNLIKELY( count + toAdd >= limit ) )\
{\
	char *tmp = (char*) realloc(buffer, 2 * limit);\
	if ( tmp == NULL )\
		printf("Problemas con realloc()\n");\
	else \
		buffer = tmp;\
	limit *= 2;\
}\
} while (0)
