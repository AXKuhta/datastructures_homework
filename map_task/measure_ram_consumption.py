import psutil
import os
from subprocess import PIPE
from time import perf_counter
from plotly.graph_objects import Scatter, Figure
from plotly.subplots import make_subplots


################################################################################################
# Запуск для разных n и получение статистики
################################################################################################

def run_test(command):
	start = perf_counter()
	p = psutil.Popen(command, stdout=PIPE, stdin=PIPE)
	
	p.stdout.read(2)

	memory = p.memory_info()
	time = perf_counter() - start

	p.stdin.write(b"\n")

	return memory.rss, memory.vms, time


cpp_x = []
cpp_series_rss = []
cpp_series_vms = []
cpp_series_time = []

py_x = []
py_series_rss = []
py_series_vms = []
py_series_time = []

# Размер теста: 100000000
# Размер шага:  20000000
# Иначе очень долго
for i in range(0, 10**8, 10**7*2):
	if i == 0:
		i = 1

	rss, vms, time = run_test(f"map_ram.exe {i}")

	cpp_series_rss.append(rss)
	cpp_series_vms.append(vms)
	cpp_series_time.append(time)
	cpp_x.append(i)

	print(f"cpp {i}")

# Размер теста: 100000
# Размер шага:  2500
for i in range(0, 10**5, 10**4//4):
	if i == 0:
		i = 1

	rss, vms, time = run_test(f"python dict_ram.py {i}")

	py_series_rss.append(rss)
	py_series_vms.append(vms)
	py_series_time.append(time)
	py_x.append(i)

	print(f"python {i}")



################################################################################################
# Построение графиков
################################################################################################

# https://plotly.com/python/multiple-axes/
fig_cpp = make_subplots(specs=[[{"secondary_y": True}]])

fig_cpp.add_traces([
	Scatter(x=cpp_x, y=cpp_series_rss, name="cpp bytes rss"),
	Scatter(x=cpp_x, y=cpp_series_vms, name="cpp bytes vms")
])

fig_cpp.add_trace(
	Scatter(x=cpp_x, y=cpp_series_time, name="cpp seconds"),
	secondary_y=True
)

fig_python = make_subplots(specs=[[{"secondary_y": True}]])

fig_python.add_traces([
	Scatter(x=py_x, y=py_series_rss, name="python bytes rss"),
	Scatter(x=py_x, y=py_series_vms, name="python bytes vms")
])

fig_python.add_trace(
	Scatter(x=py_x, y=py_series_time, name="python seconds"),
	secondary_y=True
)


################################################################################################
# Вывод в файл
################################################################################################

# По умолчанию Plotly не поддерживает форматирование шкал в объемах памяти
# Мне пришлось модифицировать его, чтобы он принимал пользовательскую функцию форматирования
# Модифицированная копия включена в этот репозиторий

# Функция на JavaScript для форматирования объемов памяти
ram_usage_format_function_js = """
const units = ["B", "KB", "MB", "GB"]
const fixed = [0, 0, 1, 2]
var mag = +(x > 1024**1) + (x > 1024**2) + (x > 1024**3) + (x > 1024**4)

return Number(x / 1024**mag).toFixed(fixed[mag]) + units[mag]
"""

# И для времени
time_format_function_js = """
const units = ["s", "ms"]
const fixed = [1, 0]
var mag = +(x < 1)

return Number(x * 1000**mag).toFixed(fixed[mag]) + units[mag]
"""

fig_cpp.layout.yaxis.tickformat = ram_usage_format_function_js
fig_cpp.layout.yaxis2.tickformat = time_format_function_js
fig_cpp.layout.yaxis2.tickmode = "sync"

fig_python.layout.yaxis.tickformat = ram_usage_format_function_js
fig_python.layout.yaxis2.tickformat = time_format_function_js
fig_python.layout.yaxis2.tickmode = "sync"

with open("memory+time.html", "w") as f:
		f.write("<!DOCTYPE html>")
		f.write("<script src=\"plotly-2.18.0-modified.js\"></script>")
		f.write("<style>article {max-width: 56em; margin: 0 auto;}</style>")
		f.write("<article>")
		f.write(fig_cpp.to_html(full_html=False, include_plotlyjs=False))
		f.write(fig_python.to_html(full_html=False, include_plotlyjs=False))
		f.write("</article>")

os.system("memory+time.html")