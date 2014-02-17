function nn = nnsetup(architecture)
%NNSETUP creates a Feedforward Backpropagate Neural Network
% nn = nnsetup(architecture) returns an neural network structure with n=numel(architecture)
% layers, architecture being a n x 1 vector of layer sizes e.g. [784 100 10]

    nn.size = architecture;
    nn.n  = numel(nn.size);
    
    %  Activation functions of hidden layers: 'sigm' (sigmoid) or 'tanh_opt' (optimal tanh).
    nn.activation_function = 'tanh_opt';
    %  learning rate Note: typically needs to be lower when using 'sigm' activation function and non-normalized inputs.
    nn.learningRate = 2;
    %  Momentum
    nn.momentum = 0.5;
    %  Scaling factor for the learning rate (each epoch)
    nn.scaling_learningRate = 1;
    %  L2 regularization
    nn.weightPenaltyL2 = 0;
    %  Non sparsity penalty
    nn.nonSparsityPenalty = 0;
    %  Sparsity target
    nn.sparsityTarget = 0.05;
    %  Used for Denoising AutoEncoders
    nn.inputZeroMaskedFraction = 0;
    %  Dropout level (http://www.cs.toronto.edu/~hinton/absps/dropout.pdf)
    nn.dropoutFraction = 0;
    %  Internal variable. nntest sets this to one.
    nn.testing = 0;
    %  output unit 'sigm' (=logistic), 'softmax' and 'linear'
    nn.output = 'sigm';

    for i = 2 : nn.n   
        % weights and weight momentum
        nn.W{i - 1} = (rand(nn.size(i), nn.size(i - 1) + 1) - 0.5) * 2 * 4 * sqrt(6 / (nn.size(i) + nn.size(i - 1)));
        nn.vW{i - 1} = zeros(size(nn.W{i - 1}));
        % average activations (for use with sparsity)
        nn.p{i}     = zeros(1, nn.size(i));   
    end
end
