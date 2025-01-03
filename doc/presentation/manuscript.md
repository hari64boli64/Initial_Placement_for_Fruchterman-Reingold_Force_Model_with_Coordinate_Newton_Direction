# manuscript

## 1

Thank you for the introduction.
Then, I'd like to start my presentation. The presentation title is Initial Placement for Fruchterman--Reingold Force Model with Coordinate Newton Direction.
And my supervisor is Professor Akiko Takeda.

## 2

To begin with, I'll introduce and summarize our study.

## 3

Firstly, please let me briefly introduce what the graph drawing is.
The graph is a mathematical structure representing pairwise relationships between objects, composed from vertices $V$ and edges $E$.
There are many examples of graphs, such as social network graphs, railroad graphs.
In particular, graph drawing is one of the most fundamental tasks in information visualization.

You are probably most familiar with graph drawing by CONNECTED PAPERS.
You might have used this site before.
When you specify a paper (like this purple one), it draws a graph with vertices as papers and edges as citations.
The drawn graph allows us to understand the flow and relevance of the research visually.
Thus, correct and beautiful Graph drawing is essential not only in the theoretical field but also in the practical field.

And, Force-directed graph drawing is one of the most popular methods for drawing graphs.

## 4

The force-directed graph drawing uses a force model composed of particles with forces acting between them.
Among the various force models, the Fruchterman--Reingold (FR) force model is the most prominent one, regarded as flexible, intuitive, and simple.

Here is an example by NetworkX, the popular graph library in Python.
So we import NetworkX as nx (at here).
`nx.draw`, as known as `nx.spring_layout`, is the function to draw a graph using the FR force model, and NetworkX draws the graphs by FR algorithm with 50 iterations.

Unfortunately, the FR algorithm has an evident drawback.
Let us consider a cycle graph example.
When the number of vertices is 10, it well visualizes the graph in 0.2 seconds. There is no problem.
When the number of vertices is 20, it starts visualizing the graph with a tangled structure in 0.2 seconds.
And, When the number of vertices is 500, although it takes as long as 11.5 seconds and the graph is just a simple cycle, the visualization result is too messy, too tangled, and too twisted.

This is exactly what we want to address in this study.

## 5

To explain this phenomenon, we introduce the FR force model.

The FR force model uses a force model with two kinds of forces: the attractive force $F^a_{i,j}$ (this one) and the repulsive force as $F_r$ (this one).

$k$ is just a constant and $d$ is the distance between vertices $i$ and $j$.

And here, $w_{i,j}$ is the weight of the edge between vertices $i$ and $j$. It has a positive value if and only if an edge between $i$ and $j$ exists.
For every pair of vertices with an edge, the attractive force pulls them together.
For every pair of vertices, whether there is an edge or not, the repulsive force pushes them apart.

When we consider only one pair of vertices, there is an equilibrium between these two forces at some distance $d$.

Then, the FR algorithm visualizes a graph by simulating the system and seeking the equilibrium of ALL of these forces.
This is how the FR algorithm works.

## 6

Now, we can see why the problem we mentioned before can happen.
The most critical issue for the FR algorithm is that twist slows down the simulation process.
The twist means unnecessary folded and tangled structures in the visualized graph.

As this gif shows, the FR algorithm from a random initial placement, the most standard way to draw a graph, stagnates at the twisted structure.

Although the optimal graph structure is just a simple cycle, the twist weakens or diminishes the forces, causing stagnation.

Further, the FR algorithm suffers from high computational complexity when directly applied, $\order{\abs{V}^2}$ per iteration, where $\abs{V}$ is the number of vertices.

Thus, we need to find a way to avoid the twist with a small computational complexity.

## 7

In previous studies, several ways to improve the FR algorithm have been proposed.

One of the strategies is to utilize L-BFGS algorithm, a family of quasi-Newton methods.
It utilizes the approximated inverse Hessians and is reported to be effective then FR algorithm.
It can overcome the twist problem to some extent.

However, it has some limitations.
Firstly, it also takes $\order{\abs{V}^2}$ per iteration.
It may fail to achieve the optimal visualization result within a limited number of iterations.
Moreover, this method just treats the problem as a general optimization problem (like this).
$x_i$ is the position of vertex $i$ and $X$ is the matrix of all of these positions.
Instead of using this matrix, it flattens into a vector, ignoring the inherent graph structure.
In particular, it just starts from a random initial placement.

So, we aim to further accelerate this optimization process.
To achieve this, we mainly focus on providing a better initial placement.

## 8

As we said, providing initial placement is another strategy for graph drawing.
Indeed, a pre-processing step with Simulated Annealing (SA) is also known to be effective since SA can avoid getting stuck in local optima and leads to a better visualization combined with the FR algorithm.
It fix the placement to a circle placement $Q^circle$ and minimize the objective function (this one) by SA.

However, this work is restricted to unweighted graphs since the objective function does not include the weight $w_{i,j}$, limited to a simple circle initial placement for this $Q$, inefficient since it just randomly selecting two vertices and test swapping, and sometimes ignoring the graph's sparsity due to this term $E_2$.

So, it leaves significant room to improve the strategy.
We aim to improve this to accelerate the subsequent optimization process,
and extend the applicability of the idea of the initial placement to weighted, complex, and large-scale graphs.

## 9

So, in this situation, we propose a new initial placement for the FR force model, as depicted in this figure.

When we start from a random initial placement, the FR and L-BFGS algorithms may stagnate at the twisted structure.

But, when we provide an initial placement (this one) with fewer twists than random placement within a short time, we can accelerate the subsequent optimization process, achieving a better visualization result.

This is the main contribution of our study.
From now on, we will explain the details of our proposed method.

## 10

Ok, then, let me explain the details of our proposed method.

## 11

Firstly, we formulate the problem.
Recall that the FR force model uses the attractive force $F^a_{i,j}$ and the repulsive force $F_r$.

Integrating the forces defines its scalar potential, the energy function.
The energy function between vertices $i$ and $j$ is defined as $E_{i,j}$, the sum of the attractive energy $E^a_{i,j}$ and the repulsive energy $E_r$.

In the FR algorithm, we seek the equilibrium of the forces by simulating the system.
In contrast to this ordinary approach, we minimize the energy function to seek the equilibrium.

Note that the minimum of $f$ yields the equilibrium positions since $\nabla f(X)$ corresponds to the forces.

## 12

Next, to introduce our proposed method, we explain the simplified problem.

Even at the expense of accuracy, obtaining an approximate solution quickly is crucial for the initial placement.
So, to obtain it, we simplify the problem into a more manageable and well-behaved discrete optimization problem, which is defined as (this one).

In this problem, we fix placement X as the subset of discrete points set $Q^hex$, the hexagonal lattice, and minimize this objective function.
(So we consider like this blue one, orange one, and green one. And the objective function is minimized from this one to this one. We aim to find the optimal assignment $X$ like this.)

Here, we explain why we use this simplified problem.
We derive this problem by simplifying the original problem, the problem (1).
We first separate $f(X)$ into attractive energy terms and repulsive energy terms.
This is equivalent to the original problem, the problem (1).

## 13

Then, we introduce the idea of initial placement.
We fix the possible positions $x_i$ to a discrete point set $Q$.
If we add a constraint, the placement is limited to $Q$; the problem is converted to this one.

As said before, thanks to the graph sparsity, the number of terms in the attractive energy, this blue one, is much less than the number of terms in the repulsive energy, this orange one.
Thus, to simplify the problem, we want to drop this second term.
When we take $Q$ such that every pair of vertices is apart by at least $\epsilon$, the second term becomes negligible.
This is because this $-k^2\log$ decreases monotonically concerning the distance.
For sufficiently large $d$, the value of $-k^2\log$ does not vary excessively.
For too small $d$, we can prevent the divergence of the energy function by setting $\epsilon$.
Thus, by dropping the second term, we can simplify the problem.

Although this $Q$ is arbitrary, since we only consider the attractive energy $f^a(X)$, the point set $Q$ should be as dense as possible.
Thus, we use the hexagonal lattice, one of the closet packing, as $Q$.

## 14

So, although the argument was somewhat complicated, this is the summary of the first half.
The essence of this conversion is just simple.

We want to solve this problem: the sum of the attractive energy terms and the repulsive energy terms.

And to drop this $\order{\abs{V}^2}$ second term, we simplify the problem by using the hexagonal lattice as the initial placement $Q$, so we skip to check the effect by the repulsive forces, which works for every pair of vertices.

Now, we will explain how to solve this problem in the second half.

## 15

To solve the problem, we can use the coordinate Newton direction.
So, to begin with, please let me introduce it in this slide.

Let us consider a strictly convex function $f$.
The second-order approximation of $f$ at $x_0$ is this one.
The minimum $x^*$ of this approximation must satisfy the stationary condition, the derivative of it equals zero.
Thus, we obtain this equation and this red term is called the Newton direction in general.
Here, please note that the Hessian matrix $\nabla^2 f(x_0)$ is positive definite since $f$ is strictly convex, so we can compute the inverse Hessian, assuring that this is a descent direction.
This is an important property for the Newton direction.

Although the Newton direction provides a critical step in iterative methods, it requires the computation of the inverse Hessian $\nabla^2 f(x_0)^{-1} \in \bbR^{n \times n}$, which is computationally very expensive.

Still, we can leverage the concept of the Newton direction in a different manner, the stochastic coordinate descent with the coordinate Newton direction.
Instead of computing the inverse Hessian $\nabla^2 f(x_0)^{-1}$ in the entire variable space $\bbR^n$, we limit the variable $x$ to its coordinate block $x_i$ with fewer dimensions, and compute this $f_i$'s Newton direction, the coordinate Newton direction.
Since the coordinate Newton direction computation is much cheaper than that of the Newton direction, we can repeat this procedure many times.
In general, this idea is known as stochastic coordinate descent or Randomized Subspace Newton (RSN) in a broader context.

In particular, this coordinate Newton direction has an apparent natural affinity to the problem.
By taking the position $x_i$ of the vertex $i$ as the coordinate block, we can compute the Newton direction.
Although directly applying this idea to the problem is challenging, we leverage this coordinate Newton direction to propose our algorithm.

## 16

Now, we will explain the proposed method.
First, by randomly taking a vertex $i$ from $V$, we compute the coordinate Newton direction for $x_i$, and its gradient and Hessian are computed as these.
Since this Hessian is just 2 by 2, we can compute the inverse Hessian easily, and thus we can obtain the coordinate Newton direction.

Since $f^a_i$ is strictly convex, the coordinate Newton direction is a descent direction.
This is different from the energy function $E_{i,j}$ and $f^a$.

## 17

Then, using this coordinate Newton direction, we update the position of $i$ as $x_i^{new}$ in the hexagonal lattice $Q^hex$, and by repeating this process, we can solve the problem.
So, here, we explain how to update the position $x_i$ in an one iteration.

If our problem is just a general continuous optimization problem, we can directly apply the coordinate Newton direction.
However, $x_i^new$ may not be in the hexagonal lattice $Q^hex$. So, we need to round it to the nearest point in $Q^hex$.
This figure shows this rounding operation. Select the vertex $i$ at $x_i$, and compute the coordinate Newton direction as this arrow.
Then, by rounding it to the nearest point in $Q^hex$, we determine the new position $x_i^{new}$. To keep the injectivity of the assignment $\pi$, we need to swap the vertices if necessary.

In this process, we empirically found that adding a small random noise vector to the coordinate Newton direction is effective.
This randomness is similar to the SA one, and it can help to escape from local minima and explore the solution space more effectively.

So, this is the one step of our proposed method, and we repeat this process for some iterations.

## 18

Finally, the obtained placement (this one) could be too small or too large since we did not care about the scale. Thus, as the final step, we rescale by $c^*$ to obtain the initial placement  (like this) and apply the L-BFGS algorithm for the final placement.

So we have to find the optimal scaling factor $c^*$. Recalling the original problem for the FR force model, the problem is to minimize this function $\phi(c)$. Here and here is the $c$.

Since this function is convex, we can find the optimal scaling factor $c^*$ by solving this problem as this one.

This value can be computed in the $\order{\abs{E}}$ complexity.
Thus, as far as we rescale the placement by this $c^*$, we can select any $\epsilon$ to define the hexagonal lattice $Q^hex$. So, we can eliminate the parameter $\epsilon$ from the problem.

## 19

Ok, now, this slide shows the pseudo-code of the proposed method.
It is just a repeat of the previous slides.

We first randomly select vertex $i$ from $V$, then compute the coordinate Newton direction and round it to $x_i^new$ in $Q^hex$, and update the position of $i$.
We repeat this process until some iterations.
Finally, we rescale by $c^*$ to obtain our proposed initial placement.

So, this is the outline of our proposed method.

## 20

Next, we will show the experimental results.

## 21

Firstly, I want show the gif of the proposed method.

Our proposed algorithm starts from a random initial placement on the hexagonal lattice $Q^hex$.
Then, it computes the coordinate Newton direction to update positions, quickly obtaining the initial placement with fewer twists.

And then, we apply L-BFGS algorithm to obtain the final placement.
This subsequent L-BFGS algorithm can achieve a better visualization result, since we provide initial placement with fewer twist, escaping the twist problem.

## 22

Next, we show more experimental results for individual graphs.

Our proposed method, using Coordinate Newton, is represented by CN in these results.

Here, in this plot, the solid lines shows the result with CN, the proposed method, and the dashed lines shows the result from a random initial placement.

As you can see, for both FR (blue lines) and L-BFGS (orange lines), the solid lines (our proposed method) are better than the dashed lines in terms of the energy function value $f(X)$.

The drawn graphs are also more resembling the optimal graph structure.

## 23

This tendency is also observed in the other graphs, with the vertices more than two thousand or more than four thousand.

Not only for L-BFGS algorithm results, for FR algorithm results, our proposed method can achieve a better visualization result.

## 24

And here is the overall comparison with various graphs.
We used matrices from the Sparse Matrix Collection as the adjacency matrices of the graphs, in total one hundred twenty four graphs.

This result shows the difference between $f(X)$ by the proposed algorithm and the random initial placement.
It shows the superior performance of our proposed method since its difference is mostly negative.

When we compare with another initial placement method, Simulated Annealing (SA), our proposed method is also better for most cases.

And at this point, dwt_992 is the case where our proposed method was worse than the SA method.
In next slide, I will explain the reason.

## 25

So here is the visualization result of dwt_992, our worst case, and the result of collins_15NN, our best case.

For the case of dwt_992, in the visualized graphs, we can not observe blue vertices. This means that the vertices are too close to each other, and the red vertices hide the blue vertices.
This is quite different property from our assumed model, every points are apart by at least $\epsilon$. Thus, our proposed method does not work well for this case.

In contrast, when we focus on collins_15NN, the best case, compared to the failure of the SA method to solve the twist, our proposed method can achieve a better visualization result, since the initial placement avoids the twist.

Given that our proposed method can be also applicable to weighted, complex, and large-scale graphs, we can conclude that our proposed method is effective for the graph drawing with FR force model.

## 26

Finally, I will discuss the future works.

## 27

Firstly, we explain the combination with the other graph visualization techniques.

sfdp is also a popular graph drawing algorithm in the Graphviz library.
It stands for Scalable Force-Directed Placement, and it is known to be effective for large-scale graphs.
This algorithm uses a multi-level approach, and it approximate graph using Barnes-Hut algorithm or Q-tree. By using this coarse approximation, it can reduce the number of vertices in the simulation.

Our algorithm can be combined with sfdp. We can apply our proposed method to the graphs in the multi-level approach.
Considering such a combination is one of the future works.

## 28

Secondly, we explain the extension to more broader applications in continuous optimization.

In general, the objective function of the problem (1) is classified as a "objective function arising from graphs," which is defined as this one.

For this general optimization problems, it is known that the stochastic coordinate descent is effective, but only with the coordinate gradient is a popular method.
We want to investigate the applicability of our proposed method, the coordinate Newton direction, to this general optimization problems.

In particular, Graph Isomorphism Problem is an interesting instance.
It has affinity to the graph drawing, since drawing graph symmetry is at least as difficult as the graph isomorphism problem.

When we relax the problem to the continuous optimization problem, the problem becomes the optimization problem on the Riemmanian manifold.

Investigating whether or not we can utilize the coordinate Newton direction for Riemmanian optimization is also one of the future works.

## 29

That is all for my presentation.
Thank you for your attention.
